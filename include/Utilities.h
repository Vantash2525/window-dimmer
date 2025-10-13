#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include <vector>
#include <windows.h>

std::vector<std::pair<HWND,std::string>> enumerate_windows();

RECT find_window_rect(HWND hwnd);

int CreateOverlay(HWND target_hwnd ,HINSTANCE hinstance , int cmdshow);

void update_overlay(HWND hwnd , HWND target_hwnd);

void set_overlay_brightness(BYTE alpha);

void set_overlay_fps(int fps);
#endif // UTILITIES_H