#include <windows.h>
#include "../include/Utilities.h"
#include <vector>
#include <string>

std::vector<std::pair<HWND,std::string>> windows;
BOOL CALLBACK EnumWindowsProc(HWND hwnd , LPARAM lparam){
    if(!IsWindowVisible(hwnd)){
        return TRUE;
    }
    char title[256];
    GetWindowTextA(hwnd,title,sizeof(title));
    if(strlen(title)>0){
        windows.push_back({hwnd,std::string(title)});
    }
    return TRUE;
}

std::vector<std::pair<HWND,std::string>> enumerate_windows(){
    EnumWindows(EnumWindowsProc,0);
    /*char buffer[256];
    for(const auto& win : windows){
        sprintf(buffer,"HWND: %p, Title: %s\n",win.first,win.second.c_str());
        MessageBoxA(NULL,buffer,"window info",MB_OK);
    }
        */
    return windows;
}