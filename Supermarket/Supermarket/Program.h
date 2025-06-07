#pragma once

#include "Response.h"
#include "Vector.h"
#include "CommandHandler.h"
#include "UserManager.h"
#include "Product.h"

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
	Vector<Product*> products_;
};