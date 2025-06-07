#include "Program.h"
#include "Cashier.h"
#include "Manager.h"

Program::Program() : commandHandler_(this->userManager_)
{
    srand(time(NULL));
    this->userManager_.loadUsers();
    this->userManager_.loadPendingUsers();
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
        this->commandHandler_.dispatch(inputs);
        std::cout << '\n';
    }
}

void Program::save()
{
    this->userManager_.uploadUsers();
    this->userManager_.uploadPendingUsers();
}
