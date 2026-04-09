#include "util.h"

int startsWith(char* a, char* b){
    size_t lena = strlen(a);
    size_t lenb = strlen(b);

    if (lenb > lena) return 0;
    for (size_t i = 0 ; i < lenb; i++){
        if (a[i] != b[i]) return 0;
    }
    return 1;
}


void copyToClipBoard(char* string){
    size_t len = strlen(string);

    HGLOBAL hgl = GlobalAlloc(GMEM_MOVEABLE, len+1);
    memcpy(GlobalLock(hgl), string, len+1);
    GlobalUnlock(hgl);

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hgl);
    CloseClipboard();
}