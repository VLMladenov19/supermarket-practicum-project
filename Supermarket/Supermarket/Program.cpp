#include "Program.h"
#include "Cashier.h"
#include "Manager.h"

Program::Program()
{
    this->userManager.loadUsers();
    this->userManager.loadPendingUsers();
}

void Program::run()
{
    std::cout << "Welcome to FMI Supermarket Management System 2024-2025!\n";
    std::cout << "Please choose an action:\n\n";

    loop();
    save();
}

Program::~Program()
{
    this->save();
}

void Program::loop()
{
    while (true)
    {
        std::cout << "> ";

        String line;
        getline(std::cin, line);

        if (line.empty())
        {
            continue;
        }

        Vector<String> inputs = line.split();
        String command = inputs[Constants::COMMAND_INDEX];
        if (command == "exit")
        {
            return;
        }
        if (command == "list-workers")
        {

        }
    }
}

void Program::save()
{
    this->userManager.uploadUsers();
    this->userManager.uploadPendingUsers();
}
