#include <fstream>
#include <iostream>
#include <Windows.h>

#include <nlohmann/json.hpp>

#include "manager.h"

std::string GetExecutablePath() 
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

void Manager::InitializeFile() noexcept
{
    nlohmann::json j;
    std::ifstream file(GetExecutablePath() + this->file_name);

    if(!file.is_open())
        return;

    try
    {
        file >> j;

        for(auto &entry : j)
            this->data.emplace_back(entry["shortcut"], entry["url"], entry["login"], entry["password"]);

        file.close();
    }
    catch (...)
    {
        std::cout << "Error while fetchin data." << std::endl;
    }

}

[[nodiscard]] std::vector<Credits> Manager::GetData() noexcept { return this->data; }