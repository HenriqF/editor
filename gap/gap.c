#include "gap.h"

//init
void initGb(GapBuffer* gb, size_t gap_size){
    if (gap_size < 1) gap_size = 1;

    gb->buffer = calloc(gap_size, gap_size*sizeof(char));
    gb->buffer_size = gap_size;
    gb->gap_size = gap_size;
    gb->gapl = 0;
    gb->gapr = gap_size-1;
}


//getting content
void printValid(char c){ 
    if (isprint(c) || c == '\n' || c == '\r' || c == '\t') printf("%c",c);
    else printf("[!]");
}

char* getText(GapBuffer gb){
    char* result = malloc(gb.buffer_size*sizeof(char)+1);

    size_t result_index = 0;
    for(size_t i = 0; i < gb.gapl; i++){
        if (gb.buffer[i] != 0) result[result_index++] = gb.buffer[i];
    }
    for(size_t i = gb.gapr+1; i < gb.buffer_size; i++){
        if (gb.buffer[i] != 0) result[result_index++] = gb.buffer[i];
    }
    result[result_index] = '\0';

    return result;
}

void renderBuff(GapBuffer gb, int s){
    for(size_t i = 0; i < gb.gapl; i++){
        printValid(gb.buffer[i]);

    }
    printf(GBBLU "<" GBRESET);
    
    if(s == 1){
        for(size_t i = gb.gapl; i < gb.gapr+1; i++) printf(GBRED "_" GBRESET);
    }
    
    for(size_t i = gb.gapr+1; i < gb.buffer_size; i++){
        printValid(gb.buffer[i]);
    }
}


//grow
void grow(GapBuffer* gb){
    char* temp = realloc(gb->buffer, gb->buffer_size+gb->gap_size);
    gb->buffer = temp;
    gb->buffer_size += gb->gap_size;

    for (size_t i = gb->buffer_size-1; i > gb->buffer_size-gb->gap_size; i--){
        gb->buffer[i] = 0;
    }

    if (gb->gapr < gb->buffer_size-gb->gap_size-1){
        size_t afteri = gb->gapl+1;
        size_t aftere = gb->buffer_size-gb->gap_size-1;
        size_t l = aftere-afteri+1;

        memmove(gb->buffer+(gb->buffer_size-l), gb->buffer+afteri, l);
        memset(gb->buffer+afteri, '\0', l);
    }

    gb->gapr += gb->gap_size;
}


//insert, delete
void insertChar(GapBuffer* gb, char c){
    if (gb->gapl == gb->gapr){
        grow(gb);
    }

    gb->buffer[gb->gapl] = c;
    gb->gapl++;
}

void insertString(GapBuffer* gb, char* text, size_t size){
    for (size_t i = 0; i < size; i++){
        insertChar(gb, text[i]);
    }
}

void deleteChar(GapBuffer* gb){
    if (gb->gapl == 0){
        return;
    }

    gb->buffer[gb->gapl-1] = 0;
    gb->gapl--;
}


//movingfs
void moveLeft(GapBuffer* gb){
    if (gb->gapl == 0){
        return;
    }

    gb->gapl--;
    char temp = gb->buffer[gb->gapl];
    gb->buffer[gb->gapl] = gb->buffer[gb->gapr];
    gb->buffer[gb->gapr] = temp;
    gb->gapr--;
}

void moveRight(GapBuffer* gb){
    if (gb->gapr == gb->buffer_size-1){
        return;
    }

    gb->gapr++;
    char temp = gb->buffer[gb->gapr];
    gb->buffer[gb->gapr] = gb->buffer[gb->gapl];
    gb->buffer[gb->gapl] = temp;
    gb->gapl++;
}