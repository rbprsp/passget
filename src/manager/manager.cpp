#include <fstream>
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <limits>
    #include <unistd.h>
#endif

#include <nlohmann/json.hpp>

#include "manager.h"

std::string GetExecutablePath() 
{
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (length == 0)
        throw std::runtime_error("Unable to get executable directory");
    std::string path(buffer, length);
    std::string::size_type pos = path.find_last_of("\\/");
    return path.substr(0, pos);
#else
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, PATH_MAX);
    if(length == -1)
        throw std::runtime_error("Unable to get executable directory");
    std::string path(buffer, length);
    return path;
#endif
}

void Manager::InitializeFile()
{
    nlohmann::json j;
    std::ifstream file(GetExecutablePath() + this->file_name);

    if(!file.is_open())
        throw std::runtime_error("File \"data.json\" not found");
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