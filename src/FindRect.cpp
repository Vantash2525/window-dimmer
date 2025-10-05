#include<windows.h>
#include "../include/Utilities.h"

RECT find_window_rect(HWND hwnd){
    RECT rect;
    if(!GetWindowRect(hwnd, &rect)){
        return (RECT){-1, -1, -1, -1};
    }
    return rect;
}