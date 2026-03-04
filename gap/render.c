#include "render.h"

#define LINEQTD 10

int handleChar(char c){
    if (isprint(c) || c == '\t' || c == '\r' || c == '\n') printf("%c",c);
    else printf("[%d]", c);
    return 0;
}



void render(GapBuffer gb){
    printf("\033[1;1H");
    system("cls");

    size_t start_index = gb.gapl;
    size_t max_index = gb.gapr+1;
    size_t line_count = 0;


    for(; (start_index > 0 && line_count <= LINEQTD); start_index--){
        if(gb.buffer[start_index] == '\n')line_count++;
    }
    if (start_index>0)start_index++;


    for(; (max_index < gb.buffer_size-1 && line_count <= LINEQTD*2); max_index++){   
        if(gb.buffer[max_index] == '\n'){
            line_count++;
        }
    }


    for(size_t i = start_index; i < gb.gapl; i++){
        handleChar(gb.buffer[i]);
    }

    printf(GBBLU CURSOR GBRESET);
    for(size_t i = gb.gapl; i < gb.gapr+1; i++) printf(GBRED "_" GBRESET);
    // printf(GBRED "(%d %d %d) " GBRESET,gb.gapl, gb.gapr, gb.buffer_size);

    for(size_t i = gb.gapr+1; i < max_index; i++){
        handleChar(gb.buffer[i]);
    }
}