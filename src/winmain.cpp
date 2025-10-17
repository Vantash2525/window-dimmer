#define UNICODE
#define _UNICODE

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <iostream>
#include "../include/resource.h"
#include "../include/Utilities.h"

#pragma comment(lib, "comctl32.lib")

HWND g_hDlg, g_hList, g_hBrightness, g_hFPS ;
HWND g_hTarget = NULL, g_hOverlay = NULL;
int g_dimLevel = 128;
int g_updateInterval = 50; 
std::vector<std::pair<HWND, std::string>> g_windows;


void PopulateWindowList(HWND hList) {
    SendMessageW(hList, LB_RESETCONTENT, 0, 0);
    g_windows = enumerate_windows();
    for (auto& win : g_windows) {
        std::wstring wtitle(win.second.begin(), win.second.end());
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)wtitle.c_str());
    }
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_INITDIALOG:
        g_hDlg = hDlg;
        g_hList = GetDlgItem(hDlg, IDC_WINDOWS_LIST);
        g_hBrightness = GetDlgItem(hDlg, IDC_BRIGHTNESS_SLIDER);
        g_hFPS = GetDlgItem(hDlg, IDC_FPS_SLIDER);

        PopulateWindowList(g_hList);

        SendMessageW(g_hBrightness, TBM_SETRANGE, TRUE, MAKELONG(0, 255));
        SendMessageW(g_hBrightness, TBM_SETPOS, TRUE, g_dimLevel);

        SendMessageW(g_hFPS, TBM_SETRANGE, TRUE, MAKELONG(10, 60));
        SendMessageW(g_hFPS, TBM_SETPOS, TRUE, 1000 / g_updateInterval);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            int sel = (int)SendMessageW(g_hList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) {
                g_hTarget = g_windows[sel].first;
                if (g_hOverlay) {
                    DestroyWindow(g_hOverlay);
                    //g_hOverlay = NULL;
                }
                g_hOverlay = CreateOverlay(g_hTarget, GetModuleHandle(NULL), SW_SHOW);
            }
            return TRUE;
        }

        case IDC_REFRESH:
            PopulateWindowList(g_hList);
            return TRUE;

        case IDCANCEL:
            if (g_hOverlay) {
                DestroyWindow(g_hOverlay);
                g_hOverlay = NULL;
            }
            return TRUE;

        case IDC_FULL_SCREEN:
            if(g_hOverlay){
                DestroyWindow(g_hOverlay);
                g_hOverlay = NULL;
            }
            g_hOverlay = CreateOverlay(NULL, GetModuleHandle(NULL), SW_SHOW);
            if(!g_hOverlay){
                MessageBoxA(NULL,"Failed to create full screen overlay","Error",MB_OK | MB_ICONERROR);  
            }
            return TRUE;
            

        }
        break;

    case WM_CLOSE: 
        if (g_hOverlay) {
            DestroyWindow(g_hOverlay);
            g_hOverlay = NULL;
        }
        EndDialog(hDlg, 0); 
        return TRUE;

    case WM_HSCROLL:
        if ((HWND)lParam == g_hBrightness && g_hOverlay) {
                int val = SendMessage(GetDlgItem(g_hDlg, IDC_BRIGHTNESS_SLIDER), TBM_GETPOS, 0, 0);
                g_dimLevel = val;
                //set_overlay_brightness((BYTE)val);
                SetLayeredWindowAttributes(g_hOverlay, RGB(0, 0, 0), g_dimLevel, LWA_ALPHA);
        } else if ((HWND)lParam == g_hFPS) {
                int val = SendMessage(GetDlgItem(g_hDlg, IDC_FPS_SLIDER), TBM_GETPOS, 0, 0);
                g_updateInterval = 1000 / val;
                KillTimer(g_hOverlay, 1);
                SetTimer(g_hOverlay, 1, g_updateInterval, NULL);
                //set_overlay_fps(val);
        }
        break;

    case IDC_FULL_SCREEN:
        if (LOWORD(wParam) == IDC_FULL_SCREEN) {
            int width = GetSystemMetrics(SM_CXSCREEN);
            int height = GetSystemMetrics(SM_CYSCREEN);
            HWND fullScreenOverlay = CreateOverlay(NULL, GetModuleHandle(NULL), SW_SHOW);
            if(!fullScreenOverlay) {
                MessageBoxA(NULL, "Failed to create full screen overlay", "Error", MB_OK | MB_ICONERROR);
            }else{
                MessageBoxA(NULL, "Full screen overlay created", "Info", MB_OK | MB_ICONINFORMATION);
            }
        }
    }

    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icex);

    int result = DialogBoxParamW(hInstance, MAKEINTRESOURCEW(IDD_MAIN_DIALOG), NULL, DlgProc, 0);
    if (result == -1) {
        MessageBoxA(NULL, "DialogBoxParamW failed — resource not found", "Error", MB_OK | MB_ICONERROR);
    }

    return 0;
}
