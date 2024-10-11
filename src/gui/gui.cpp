#include <Windows.h>
#include <fstream>
#include <iostream>

#include <ftxui/component/component.hpp>
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
    for (const auto& entry : _data)
        entries.push_back(entry.shortcut);

    auto screen = ScreenInteractive::Fullscreen();

    int shortcut_selected = 0;
    int copy_selected = 0;

    bool show_copy_menu = false;

    auto shortcut_menu = Menu(&entries, &shortcut_selected);
    std::vector<std::string> copy_entries = {
        "Name:   " + this->_data[this->entry].shortcut,
        "URL:    " + this->_data[this->entry].url,
        "Login:  ********",
        "Pass:   ********",
    };
    auto copy_menu = Menu(&copy_entries, &copy_selected);

    auto container = Container::Vertical({
        shortcut_menu,
        copy_menu,
    });

    auto render = Renderer(container, [&] {
        if (show_copy_menu) {
            return vbox({
                text("Your Data: ") | bold,
                copy_menu->Render(),
                text("Press 'q' to go back")
            });
        } else {
            return vbox({
                text("passget v1.0.0") | bold,
                shortcut_menu->Render(),
                text("Press 'Enter' to select")
            });
        }
    });

    auto main_component = CatchEvent(render, [&](Event event) 
    {
        if (event == Event::Return) 
        {
            if (!show_copy_menu) 
            {
                this->entry = shortcut_selected;
                show_copy_menu = true;
                copy_entries = {
                    "Name:   " + this->_data[this->entry].shortcut,
                    "URL:    " + this->_data[this->entry].url,
                    "Login:  ********",
                    "Pass:   ********",
                };
                copy_menu = Menu(&copy_entries, &copy_selected);
                container->DetachAllChildren();
                container->Add(shortcut_menu);
                container->Add(copy_menu);
            } 
            else 
            {
                switch (copy_selected) 
                {
                    case 1:
                        ShellExecute(0, 0, this->_data[this->entry].url.c_str(), 0, 0, SW_SHOW);
                        break;
                    case 2:
                        CopyToClipboard(this->_data[this->entry].login);
                        break;
                    case 3:
                        CopyToClipboard(this->_data[this->entry].password);
                        break;
                    default:
                        break;
                }
            }
            return true;
        } 
        else if (event == Event::Character('q')) 
        {
            show_copy_menu = false;
            copy_selected = 0;
            screen.PostEvent(Event::Custom);
            return true;
        }
        return false;
    });

    screen.Loop(main_component);
}

void GUI::AddData() noexcept
{
    std::string shortcut;
    std::string url;
    std::string login;
    std::string password;
    std::string info_text = "";

    bool saved = false;

    Component input_shortcut = Input(&shortcut, "Shortcut");
    Component input_url = Input(&url, "URL");
    Component input_login = Input(&login, "Login");
    Component input_password = Input(&password, "Password");

    auto screen = ScreenInteractive::TerminalOutput();

    Component button_exit = Button("Save", [&] 
    {
        if(shortcut != "" && login != "" && password != "")
        {
            if(url == "")
                url = " ";

            AddToFile(shortcut, url, login, password);
            info_text = " Saved";
            screen.Exit();
        }
        else
            info_text = " Fill all fields";
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
            hbox(text(info_text)),
            button_exit->Render()
        }) | border;
    });

    screen.Loop(renderer);
}