#include "rw/readwrite.h"
#include "gap/gap.h"
#include "gap/render.h"

#include <windows.h>
#include <conio.h>

#define VER "04.03.2026.1"

GapBuffer editor;
char* editor_file_path;

GapBuffer bar;

GapBuffer* gb;
int show_gap_buffer = 0;

int down_offset = 0;
int command_bar_mode = 0;

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
            insertChar(gb, '\n');
            break;
        case -121:
            insertChar(gb, -61);
            insertChar(gb, -89);
            break;
        case 8:
            deleteChar(gb);
            break;
        default:
            insertChar(gb, c);
    }
    
    render(*gb, show_gap_buffer, down_offset);
}

void handleSPInput(char c){
    if (c == 75) moveLeft(gb);
    else if (c == 77) moveRight(gb);
    else if (c == 72) moveUp(gb);
    else if (c == 80) moveDown(gb);

    render(*gb, show_gap_buffer, down_offset);
}


typedef enum {
    CONT,
    BREAK,
    EXIT,
} BarAcReturn;

BarAcReturn handleBarActions(char* command){
    if (strcmp(command, "s") == 0 || strcmp(command, "S") == 0){
        gb = &editor;

        FILE* f = fopen(editor_file_path, "wb");
        if (!f){
            printf("\narquivo nao encontrado.");
            return CONT;
        }
        char* text = getText(*gb);
        writeFile(f, text);
        fclose(f);
        free(text);
        printf("\nSalvo em: %s", editor_file_path);

        gb = &bar;
        return CONT;
    }

    if (strcmp(command, "g") == 0 || strcmp(command, "G") == 0){
        show_gap_buffer = !show_gap_buffer;
    }
    
    if (strcmp(command, "v") == 0 || strcmp(command, "V") == 0){
        gb = &editor;
        down_offset = 0;

        system("cls");
        render(*gb, show_gap_buffer, down_offset);
        return BREAK;
    }

    if (strcmp(command, "sair") == 0){
        return EXIT;
    }

    
    return CONT;
}


void loadFile(GapBuffer* gb, char* path){
    size_t content_size;
    char* content;

    FILE* f;
    f = fopen(path, "rb");
    if (!f){
        printf("arquivo nao encontrado.");
        return;
    }
    readFile(f, &content_size, &content);
    fclose(f);
    insertString(gb, content, content_size);
}


int main(int argc, char** argv){
    SetConsoleOutputCP(CP_UTF8);
    printf("\e[1;1H\e[2J");
    printf("\033[?25l");

    initGb(&editor, 100);
    initGb(&bar, 20);

    gb = &editor;


    if (argc == 2){
        editor_file_path = argv[1];
        loadFile(gb, editor_file_path);
    }

    //moveStart(gb);
    render(*gb, show_gap_buffer, down_offset);

    while (1) {
        if (_kbhit()){
            char c = _getch();

            if (command_bar_mode && c == 13){
                char* bar_text = getText(*gb);

                BarAcReturn r = handleBarActions(bar_text);
                free(bar_text);
                if (r == BREAK){
                    system("cls");
                    gb = &editor;
                    down_offset = 0;
                    command_bar_mode = 0;
                    render(*gb, show_gap_buffer, down_offset);
                }
                if (r == EXIT){
                    system("cls");
                    printf("\033[?25h");
                    break;
                }
                

                initGb(&bar, 20);
                system("cls");
                render(*gb, show_gap_buffer, down_offset);
            }

            else if (c == 27) { //entrar bar mode
                system("cls");
                gb = &bar;
                down_offset = 8;
                command_bar_mode = 1;
                
                render(*gb, show_gap_buffer, down_offset);
            }

            else if (c == -32){
                handleSPInput(_getch());
            }
            else handleKBInput(c);

            if (command_bar_mode){
                printf(
                    "versão " VER ". Comandos: \n"
                    "Salvar: 'S' | 's'.\n"
                    "Voltar ao editor: 'V' | 'v'.\n"
                    "Para mostrar/esconder o buffer: 'G' | 'g'.\n"
                    "Para sair: 'sair'."
                );
            }
        }
        
    }
    return 0;
}