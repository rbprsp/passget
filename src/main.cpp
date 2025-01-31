#include <iostream>

#include "cli.h"
#include "manager.h"

int main(int argc, char** argv)
{
    bool add = false;

    if(argc > 1)
    {
        std::string command = argv[1];
        if(command.find("--add") != std::string::npos)
            add = true;
    }


    Manager mng;
    mng.InitializeFile();

    CLI cli(mng.GetData());
    

    if(add)
    {
        cli.AddData();
    }
    else
    {
        cli.DrawMenu();
    }
    return 0;
}