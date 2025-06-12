#include "../Core/Program.h"
#include "../Users/Cashier.h"
#include "../Users/Manager.h"

Program::Program() 
    : commandHandler_(this->userManager_, this->productManager_)
{
    srand(static_cast<unsigned int>(time(NULL)));
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
        for (size_t i = 1; i < inputs.size(); ++i)
        {
            String& input = inputs[i];
            if (input.size() >= 2 && 
                input[0] == '<' && input[input.size() - 1] == '>')
            {
                input.erase(0, 1); // Remove the '<'
                input.pop_back(); // Remove the '>'
            }
        }
        this->commandHandler_.dispatch(inputs);
        std::cout << '\n';
    }
}

void Program::save()
{
    this->userManager_.uploadUsers();
    this->userManager_.uploadPendingUsers();
}
