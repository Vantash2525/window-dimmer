#include <windows.h>
#include "../include/Utilities.h"

LRESULT CALLBACK WindowProc(HWND hwnd , UINT umsg , WPARAM wparam , LPARAM lparam ){
    switch(umsg){

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:{
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);
            FillRect(hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW+1));
            EndPaint(hwnd,&ps);
            return 0;
        }
        default:
            return DefWindowProc(hwnd,umsg,wparam,lparam);
        
    }
}
int CreateOverlay(HWND target_hwnd , HINSTANCE hinstance , int cmdshow){
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

    HWND hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT, L"MyWindowClass", L"My Window", WS_POPUP, x, y, width, height, target_hwnd, NULL, hinstance, NULL);

    SetLayeredWindowAttributes(hwnd, 0, 128, LWA_ALPHA);

    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);
    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}