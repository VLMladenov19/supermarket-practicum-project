#pragma once

#include "../Utils/Vector.h"
#include "../Core/CommandHandler.h"
#include "../Managers/UserManager.h"
#include "../Managers/ProductManager.h"

class Program
{
public:
	Program();

	void run();

	~Program();

private:
	void loop();
	void save();

	CommandHandler commandHandler_;
	UserManager userManager_;
	ProductManager productManager_;
};