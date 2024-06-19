#include <Windows.h>
#include <fstream>
#include <iostream>

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Menu
#include "ftxui/component/component_options.hpp"   // for MenuOption

#include <nlohmann/json.hpp>

#include "gui.h"

#define FILENAME "/data.json"

std::string GetExePath() 
{
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (length == 0) {
        throw std::runtime_error("Unable to get executable directory");
    }
    std::string fullPath(buffer, length);
    std::string::size_type pos = fullPath.find_last_of("\\/");
    return fullPath.substr(0, pos);
}

void AddToFile(std::string shortcut, std::string url, std::string login, std::string pass)
{
    nlohmann::json j;
    std::ifstream rfile(GetExePath() + FILENAME);

    try
    {
        rfile >> j;
        rfile.close();
    }
    catch (...)
    {

    }

    nlohmann::json oj;
    std::ofstream wfile(GetExePath() + FILENAME);

    oj["shortcut"] = shortcut;
    oj["url"] = url;
    oj["login"] = login;
    oj["password"] = pass;

    j.push_back(oj);

    wfile << std::setw(4) << j;

    wfile.close();
    
}

void CopyToClipboard(const std::string& text) 
{
    if (!OpenClipboard(nullptr)) 
        return;
    
    EmptyClipboard();

    size_t size = (text.size() + 1) * sizeof(char);

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hMem) 
    {
        CloseClipboard();
        return;
    }

    memcpy(GlobalLock(hMem), text.c_str(), size);
    GlobalUnlock(hMem);

    SetClipboardData(CF_TEXT, hMem);

    CloseClipboard();
}

using namespace ftxui;

void GUI::DrawMenu() noexcept 
{
    std::vector<std::string> entries;

    for (auto& d : this->_data)
        entries.push_back(d.shortcut);

    auto screen = ScreenInteractive::TerminalOutput();
    int selected_entry = 0;

    MenuOption option;
    option.on_enter = [&] {
        this->entry = selected_entry;  // Save the selected entry
        screen.ExitLoopClosure()();
       
    };

    auto menu = Menu(&entries, &selected_entry, option);
    screen.Loop(menu);

    system("cls");
    DrawCopyMenu(screen);
}

void GUI::DrawCopyMenu(ftxui::ScreenInteractive& screen) noexcept 
{
    std::vector<std::string> copy_entries = 
    {
        "Name:   " + this->_data[this->entry].shortcut,
        "URL:    " + this->_data[this->entry].url,
        "Login:  ********",
        "Pass:   ********",
        "back",
        "exit"
    };

    int selected_copy_entry = 0;

    MenuOption copy_option;
    copy_option.on_enter = [&] 
    {
        switch (selected_copy_entry) 
        {
            case 0:
                break;
            case 1:
                ShellExecute(0, 0, this->_data[this->entry].url.c_str(), 0, 0 , SW_SHOW );
                break;
            case 2:
                CopyToClipboard(this->_data[this->entry].login);
                break;
            case 3:
                CopyToClipboard(this->_data[this->entry].password);
                break;
            case 4:
                screen.ExitLoopClosure()();
                DrawMenu();  // Go back to main menu
                return;
            case 5:
                screen.Exit();
                std::exit(0);
                break;
            default:
                break;
        }
    };

    auto copy_menu = Menu(&copy_entries, &selected_copy_entry, copy_option);
    screen.Loop(copy_menu);
}

void GUI::AddData() noexcept
{
    // The data:
    std::string shortcut;
    std::string url;
    std::string login;
    std::string password;

    Component input_shortcut = Input(&shortcut, "Shortcut");
    Component input_url = Input(&url, "URL");
    Component input_login = Input(&login, "Login");
    Component input_password = Input(&password, "Password");

    Component button_exit = Button("Exit", [&] 
    {
        if(shortcut != "" && login != "" && password != "")
        {
            if(url == "")
                url = " ";

            AddToFile(shortcut, url, login, password);
        }
        else
            std::cout << "Not all fields are filled." << std::endl;
        std::exit(0);
    });

    auto component = Container::Vertical({
        input_shortcut,
        input_url,
        input_login,
        input_password,
        button_exit,
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
            hbox(text(" Shortcut : "), input_shortcut->Render()),
            hbox(text(" URL : "), input_url->Render()),
            hbox(text(" Login : "), input_login->Render()),
            hbox(text(" Password : "), input_password->Render()),
            button_exit->Render()
        }) | border;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    screen.Loop(renderer);
}

