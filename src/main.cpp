#include "../include/Utilities.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
int main(){
    std::vector<std::pair<HWND,std::string>> windows = enumerate_windows();
    for (size_t i = 0; i < windows.size(); ++i) {
        const auto& window = windows[i];
        std::cout << i << " Window Title: " << window.second << ", HWND: " << window.first << std::endl;
    }
    std::cout << "total windows found:"<< windows.size()<<std::endl;
    std::cout << "enter the index of the window to dim: ";
    size_t index;
    std::cin >> index;
    if(index >= windows.size()){
        std::cerr<<"invalid index"<<std::endl;
        return 1;
    }
    HWND target_hwnd = windows[index].first;
    HINSTANCE hinstance = GetModuleHandle(NULL);
    int cmdshow = SW_SHOW;
    CreateOverlay(target_hwnd, hinstance, cmdshow);
    return 0;
}