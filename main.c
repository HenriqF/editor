#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

//coisa do arquivo

typedef struct{
    int length;
    char* conetudo;
}linha;

typedef struct{
    int qtdlinhas;
    linha* linhas;
}arquivo;
arquivo arq;

char* content;
size_t size;


void abrirarquivo(){
    FILE* file = fopen("read.txt", "rb");
    if (!file){
        return;
    }
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    content = malloc(size+1);
    if (content){
        size = fread(content, 1, size, file);
        content[size] = '\0';
    }
    fclose(file);
}
void salvararquivo(){
    FILE* file = fopen("read.txt", "wb");
    if (!file){
        return;
    }
    for(int i = 0; i < arq.qtdlinhas; i++){
        fprintf(file, "%s", arq.linhas[i].conetudo);
    }
    fclose(file);
}



void addLine(int starti, int endi){
    int len = endi-starti;
    char* noval = malloc(len+1);
    memcpy(noval, content+starti, len);
    noval[len] = '\0';
    linha l = {len, noval};

    linha* buff = realloc(arq.linhas, (arq.qtdlinhas+1)*sizeof(linha));
    arq.linhas = buff;
    arq.linhas[arq.qtdlinhas] = l;
    arq.qtdlinhas++;
}

void getarqstruct(){
    int i = 0;
    int lineStartIndex = 0;

    while (i < size){
        if (content[i] == '\n'){
            addLine(lineStartIndex, i+1);
            lineStartIndex=i+1;
        }
        i++;
    }
    addLine(lineStartIndex, i);
}

void listLines(){
    for(int i = 0; i < arq.qtdlinhas; i++){
        printf("%s", arq.linhas[i].conetudo);
    }
}
////

//coisa do console

CONSOLE_SCREEN_BUFFER_INFO csbi;
int cols, rows;
void updateConsoleSize(){
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

static DWORD oldmode;
void rawMode() {
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(h, &oldmode);
    DWORD m = oldmode;
    m &= ~ENABLE_ECHO_INPUT;
    m &= ~ENABLE_LINE_INPUT;
    m &= ~ENABLE_PROCESSED_INPUT;
    m &= ~ENABLE_WINDOW_INPUT;
    m &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(h, m);
}
void cookMode() {
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(h, oldmode);
}

HANDLE h;
INPUT_RECORD r;
DWORD n;
int getCharInput(){
    ReadConsoleInput(h, &r, 1, &n);
    if (r.EventType == KEY_EVENT && r.Event.KeyEvent.bKeyDown) {
        int c = r.Event.KeyEvent.uChar.AsciiChar;
        if (c == 27){
        }
        return(c);
    }
}

////


int cols, rows;



int main(){
    rawMode();
    abrirarquivo();
    getarqstruct();
    listLines();
    salvararquivo();

    cookMode();
    return 0;
}