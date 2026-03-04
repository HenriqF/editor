#ifndef GAPBUFF
#define GAPBUFF

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define GBRED "\x1b[31m"
#define GBBLU "\x1b[34m"
#define GBRESET "\x1b[0m"

#define CURSOR "<"

typedef struct{
    char* buffer;
    size_t buffer_size;
    size_t gap_size; //const

    size_t gapl; //index gapleft
    size_t gapr; //gapright
} GapBuffer;

void initGb(GapBuffer* gb, size_t gap_size);

//getting content
char* getText(GapBuffer gb);

//grow
void grow(GapBuffer* gb);

//insert, delete
void insertChar(GapBuffer* gb, char c);
void insertString(GapBuffer* gb, char* text, size_t size);
void deleteChar(GapBuffer* gb);

//movingfs
void moveLeft(GapBuffer* gb);
void moveRight(GapBuffer* gb);

void moveUp(GapBuffer* gb);
void moveDown(GapBuffer* gb);

void moveStart(GapBuffer* gb);

#endif