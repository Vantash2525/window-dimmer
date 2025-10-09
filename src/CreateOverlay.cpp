#include <windows.h>
#include "../include/Utilities.h"

static HWND target_hwnd_global = NULL;

void update_overlay(HWND hwnd, HWND target_hwnd);

LRESULT CALLBACK WindowProc(HWND hwnd , UINT umsg , WPARAM wparam , LPARAM lparam ){
    switch(umsg){
        case WM_TIMER:{
            if(IsWindowVisible(target_hwnd_global) && IsWindow(target_hwnd_global)){
                update_overlay(hwnd,target_hwnd_global);
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:{
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);
            HBRUSH hbrush = CreateSolidBrush(RGB(0,0,0));

            FillRect(hdc,&ps.rcPaint,hbrush);
            DeleteObject(hbrush);
            EndPaint(hwnd,&ps);
            return 0;
        }
        default:
            return DefWindowProc(hwnd,umsg,wparam,lparam);
        
    }
}
int CreateOverlay(HWND target_hwnd , HINSTANCE hinstance , int cmdshow){
    target_hwnd_global = target_hwnd;
    RECT rect;
    GetWindowRect(target_hwnd, &rect);
    int x = rect.left;int y = rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT |WS_EX_TOOLWINDOW | WS_EX_TOPMOST, L"MyWindowClass", L"My Window", WS_POPUP, x, y, width, height, NULL, NULL, hinstance, NULL);
    
    SetWindowLongPtr(hwnd,GWLP_HWNDPARENT,(LONG_PTR)target_hwnd);

    SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 200, LWA_ALPHA);
    
    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);

    SetTimer(hwnd,1,100,NULL);
    

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void update_overlay(HWND hwnd , HWND target_hwnd){
    if(!IsWindow(target_hwnd)) return;
    RECT rc;
    GetWindowRect(target_hwnd,&rc);
    int x= rc.left;
    int y= rc.top;
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    SetWindowPos(hwnd,HWND_TOPMOST,x,y,width,height,SWP_NOZORDER | SWP_NOACTIVATE);
}