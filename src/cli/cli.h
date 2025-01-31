#ifndef __CLI_H__
#define __CLI_H__

#include <vector>

#include <ftxui/component/screen_interactive.hpp>

#include "utils.h"

class CLI
{
private:
    std::vector<Credits> _data;
    int entry = 0;

public:
    CLI(std::vector<Credits> data) : _data(data) {} 
    void DrawMenu() noexcept;
    void DrawCopyMenu(ftxui::ScreenInteractive& screen) noexcept;

    void AddData() noexcept;

};

#endif