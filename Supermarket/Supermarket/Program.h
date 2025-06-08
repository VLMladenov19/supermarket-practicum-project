#pragma once

#include "Vector.h"
#include "CommandHandler.h"
#include "UserManager.h"
#include "ProductManager.h"

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