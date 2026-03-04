#include "rw/readwrite.h"
#include "gap/gap.h"
#include "gap/render.h"

#include <windows.h>
#include <conio.h>

#define VER "03.03.2026.1"

GapBuffer gb;
int show_gap_buffer = 0;

/*
    muito maneiro:
    salvar e carregar posicao cursor
        printf("\033[s");
        printf("\033[u");

    esconder mostrar cursor
        printf("\033[?25l");
        printf("\033[?25h");
*/


void handleKBInput(char c){

    switch (c){
        case 13:
            insertChar(&gb, '\n');
            break;
        case -121:
            insertChar(&gb, -61);
            insertChar(&gb, -89);
            break;
        case 8:
            deleteChar(&gb);
            break;
        default:
            insertChar(&gb, c);
    }
    
    render(gb, show_gap_buffer);
}

void handleSPInput(char c){
    if (c == 75) moveLeft(&gb);
    else if (c == 77) moveRight(&gb);
    else if (c == 72) moveUp(&gb);
    else if (c == 80) moveDown(&gb);

    render(gb, show_gap_buffer);
}

int handleInputAfterESC(char** argv){
    while (1) if (_kbhit()){
        char c = _getch();

        if (c == 83 || c == 115){//s
            FILE* f = fopen(argv[1], "wb");
            if (!f){
                printf("\narquivo nao encontrado.");
                continue;
            }
            writeFile(f, getText(gb));
            fclose(f);
            printf("\nSalvo em: %s", argv[1]);
        }
        else if (c == 86 || c == 118){//v
            system("cls");
            render(gb, show_gap_buffer);
            break;
        }
        else if (c == 71 || c == 103){
            show_gap_buffer = !show_gap_buffer;
            if (show_gap_buffer){
                printf("\nMostrando buffer.");
            }
            else{
                printf("\nBuffer escondido.");
            }
        }
        else if (c == 27){
            printf("\033[?25h");
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv){
    SetConsoleOutputCP(CP_UTF8);
    printf("\e[1;1H\e[2J");
    printf("\033[?25l");

    initGb(&gb, 100);

    size_t content_size;
    char* content;
    FILE* f;
    if (argc == 2){
        f = fopen(argv[1], "rb");
        if (!f){
            printf("arquivo nao encontrado.");
            return 0;
        }
        readFile(f, &content_size, &content);
        fclose(f);
        insertString(&gb, content, content_size);
    }

    //moveStart(&gb);
    render(gb, show_gap_buffer);

    while (1) {
        if (_kbhit()){
            char c = _getch();

            if (c == 27) {
                system("cls");
                printf("versão " VER "\n");

                printf("Para salvar, aperte 'S'.\nPara mostrar/esconder o buffer, aperte 'G'.\n\nPara voltar, aperte 'V'.\nPara sair, aperte 'ESC' novamente.");
                int r = handleInputAfterESC(argv);
                if (r) break;
                
            }

            else if (c == -32){
                handleSPInput(_getch());
            }
            else handleKBInput(c);
        }
        
    }
    return 0;
}