#include <windows.h>
#include "../include/Utilities.h"


static HWND target_hwnd_global = NULL;
static BYTE overlay_alpha = 128;   // default brightness
static int overlay_fps = 10;       // default FPS
static HWND overlay_hwnd = NULL;

void update_overlay(HWND hwnd, HWND target_hwnd);

LRESULT CALLBACK OverlayProc(HWND hwnd , UINT umsg , WPARAM wparam , LPARAM lparam ){
    switch(umsg){
        case WM_TIMER:{
            if(wparam ==1){
                if(target_hwnd_global==NULL){
                    update_overlay(hwnd,NULL);
                    return 0;
                }
                else if(IsWindowVisible(target_hwnd_global) && IsWindow(target_hwnd_global)){
                    update_overlay(hwnd,target_hwnd_global);
                }
                else{
                    PostMessage(hwnd,WM_CLOSE,0,0);
                }

            return 0;
            }
        }
        case WM_DESTROY:
            overlay_hwnd = NULL;
            //PostQuitMessage(0);
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
HWND CreateOverlay(HWND target_hwnd , HINSTANCE hinstance , int cmdshow){

    int x=0,y=0,width=0,height=0;
    
    target_hwnd_global = target_hwnd;
    
    if(target_hwnd){
        RECT rect;
        GetWindowRect(target_hwnd, &rect);
        x = rect.left;y = rect.top;
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }else{
        x=0;y=0;
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    }

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = OverlayProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT |WS_EX_TOOLWINDOW , L"MyWindowClass", L"My Window", WS_POPUP, x, y, width, height, NULL, NULL, hinstance, NULL);
    overlay_hwnd = hwnd;
    SetWindowLongPtr(hwnd,GWLP_HWNDPARENT,(LONG_PTR)target_hwnd);

    SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 128, LWA_ALPHA);
    
    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);

    SetTimer(hwnd,1,10,NULL);
    /*MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }*/

    return hwnd;
}

void update_overlay(HWND hwnd , HWND target_hwnd){
    if(target_hwnd==NULL){
        // Full screen mode
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hwnd,HWND_TOPMOST,0,0,width,height, SWP_NOACTIVATE);
        return;
    }
    RECT rc;
    GetWindowRect(target_hwnd,&rc);
    int x= rc.left;
    int y= rc.top;
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    SetWindowPos(hwnd,HWND_TOPMOST,x,y,width,height,SWP_NOZORDER | SWP_NOACTIVATE);
}



/*void set_overlay_brightness(BYTE alpha){
    overlay_alpha = alpha;
    if(overlay_hwnd){
        SetLayeredWindowAttributes(overlay_hwnd, RGB(0,0,0), overlay_alpha, LWA_ALPHA);
    }
}*/

/*void set_overlay_fps(int fps){
    overlay_fps = fps;
    if(overlay_hwnd){
        KillTimer(overlay_hwnd, 1);
        SetTimer(overlay_hwnd, 1, 1000 / overlay_fps, NULL);
    }
}*/