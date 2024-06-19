#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <string>

#include "utils.h"

class Manager
{
private:
    std::vector<Credits> data;

    std::string file_name = "/data.json";

public:
    void InitializeFile() noexcept;
    [[nodiscard]] std::vector<Credits> GetData() noexcept;
};

#endif