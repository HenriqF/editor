#include "render.h"

#define LINEQTD 8
#define LINEPADDING 5
void printDown(){
    for (int i = 0; i < 20; i++) printf("\n");
}

int relative_mode = 0;
int current_line;
int above_count;
int below_count;
void printLine(){
    printf(GBLGRAY);

    int current = 0;

    if (relative_mode){
        if (above_count > 0){
        current = above_count;
        above_count--;
        }
        else{
            current = below_count++;
        }
        if (current == 0){
            printf(GBRESET GBRED);
            current = current_line;
        } 
    }
    else{
        if (above_count > 0){
        current = current_line-above_count;
        above_count--;
        }
        else{
            current = current_line+below_count++;
        }
        if (current == current_line){
            printf(GBRESET GBRED);
        } 
    }

    int length = snprintf(NULL, 0, "%d", current);
    printf("%d", current);
    for (int i = length; i < LINEPADDING; i++)printf(" ");


    printf(GBRESET " ");
}

int handleChar(char c){
    if (c == '\r');
    else if (c == '\n'){
        printf("%c",c);
        printLine();
    }
    else if (isprint(c) || c == '\t' || c < 0) printf("%c",c);
    else printf("[%d]", c);
    return 0;
}


int render_down = 0;
void render(GapBuffer gb, int s, int down_offset){
    above_count = 0;
    below_count = 0;
    current_line = 0;

    printf("\033[1;1H");
    system("cls");
    for (int i = 0; i < down_offset; i++) printf("\n");

    size_t start_index = gb.gapl;
    size_t max_index = gb.gapr+1;
    size_t line_count = 0;


    for(size_t i = 0; i < start_index; i++) if(gb.buffer[i] == '\n') current_line++;

    for(; (start_index > 0 && line_count <= LINEQTD); start_index--){
        if(gb.buffer[start_index] == '\n'){
            line_count++;
            above_count++;
        }
    }
    if (line_count > LINEQTD) start_index+=2;


    for(; (max_index < gb.buffer_size && line_count <= LINEQTD*2+1); max_index++){   
        if(gb.buffer[max_index] == '\n')line_count++;
    }


    if (above_count > LINEQTD) above_count = LINEQTD;


    
    //render 
    printLine();

    for(size_t i = start_index; i < gb.gapl; i++){
        handleChar(gb.buffer[i]);
    }

    printf(GBRED CURSOR GBRESET);
    if (s){
        for(size_t i = gb.gapl; i < gb.gapr+1; i++) printf(GBRED "_" GBRESET);
        printf(GBRED "(%lld %lld %lld) " GBRESET,gb.gapl, gb.gapr, gb.buffer_size);
    }


    for(size_t i = gb.gapr+1; i < max_index; i++){
        handleChar(gb.buffer[i]);
    }
    printf("\033[1;1H");

}
