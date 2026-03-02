#include "rw/readwrite.h"
#include "gap/gap.h"

#include <unistd.h>
#include <windows.h>
#include <conio.h>

#define VER "02.03.2026.1"

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
    if (c == 13) insertChar(&gb, '\n');

    else if (c == 8) deleteChar(&gb);

    else insertChar(&gb, c);
    
    //printf("\033[2J\033[H");
    system("cls");
    renderBuff(gb, show_gap_buffer);
}

void handleSPInput(char c){
    if (c == 75) moveLeft(&gb);
    else if (c == 77) moveRight(&gb);

    //printf("\033[2J\033[H");
    system("cls");
    renderBuff(gb, show_gap_buffer);
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
            renderBuff(gb, show_gap_buffer);
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
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv){
    SetConsoleOutputCP(CP_UTF8);
    system("cls");
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

    renderBuff(gb, show_gap_buffer);;

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