#ifndef __GUI_H__
#define __GUI_H__

#include <vector>

#include <ftxui/component/screen_interactive.hpp>

#include "utils.h"

class GUI
{
private:
    std::vector<Credits> _data;
    int entry = 0;

    

public:
    GUI(std::vector<Credits> data) : _data(data) {} 
    void DrawMenu() noexcept;
    void DrawCopyMenu(ftxui::ScreenInteractive& screen) noexcept;

    void AddData() noexcept;

};

#endif