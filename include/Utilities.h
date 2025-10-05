#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include <vector>
#include <windows.h>
std::vector<std::pair<HWND,std::string>> enumerate_windows();
RECT find_window_rect(HWND hwnd);
int CreateOverlay(HWND target_hwnd ,HINSTANCE hinstance , int cmdshow);

#endif // UTILITIES_H