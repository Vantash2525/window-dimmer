#include <windows.h>
#include "../include/Utilities.h"
#include <vector>
#include <string>

std::vector<std::pair<HWND,std::string>> windows;
BOOL CALLBACK EnumWindowsProc(HWND hwnd , LPARAM lparam){

    if (!IsWindowVisible(hwnd) || !IsWindowEnabled(hwnd))
        return TRUE;

    LONG style = GetWindowLong(hwnd, GWL_STYLE);

    if(style & WS_EX_TOOLWINDOW)
        return TRUE;

    wchar_t windowclass[256];
    GetClassNameW(hwnd,windowclass,sizeof(windowclass)/sizeof(wchar_t));
    const wchar_t* excludedclasses[] = {
        L"WorkerW",              // Desktop/Shell worker windows
        L"Progman",              // Program Manager (Desktop)
        L"Shell_TrayWnd",        // Taskbar
        L"StartMenuExperienceHost",
        L"Windows.UI.Core.CoreWindow", // UWP App/System UI
        L"ApplicationFrameWindow", // UWP container window (often listed twice)
        L"MsoCFHeC",             // Office related invisible window
        L"GHOST_CLASS",          // Used by various background processes
        L"TransparentWindow",    // Used by screen recording/utility apps
        L"Program Manager",      // Sometimes listed by title/class
        L"ProgramManager"        // Another Program Manager variant
    };

    for(const auto& exclclass : excludedclasses){
        if(wcscmp(windowclass,exclclass)==0){
            return TRUE;
        }
    }
    

    char title[256];
    GetWindowTextA(hwnd,title,sizeof(title));
    if(strlen(title)>0){
        windows.push_back({hwnd,std::string(title)});
    }
    return TRUE;
}

std::vector<std::pair<HWND,std::string>> enumerate_windows(){
    windows.clear();
    EnumWindows(EnumWindowsProc,0);
    /*char buffer[256];
    for(const auto& win : windows){
        sprintf(buffer,"HWND: %p, Title: %s\n",win.first,win.second.c_str());
        MessageBoxA(NULL,buffer,"window info",MB_OK);
    }
        */
    return windows;
}