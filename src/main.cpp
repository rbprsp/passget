#include <iostream>

#include "gui.h"
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


    Manager *mng = new Manager();
    mng->InitializeFile();

    GUI *gui = new GUI(mng->GetData());
    

    if(add)
    {
        gui->AddData();
    }
    else
    {
        gui->DrawMenu();
    }
    return 0;
}