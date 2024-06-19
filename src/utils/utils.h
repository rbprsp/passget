#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

struct Credits
{
    std::string shortcut;
    std::string url;
    std::string login;
    std::string password;

    Credits(std::string _shortcut, std::string _url, std::string _login, std::string _password) : shortcut(_shortcut), url(_url), login(_login), password(_password) {}
};

#endif
