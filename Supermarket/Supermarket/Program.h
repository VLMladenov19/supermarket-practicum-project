#pragma once

#include "Response.h"
#include "Vector.h"
#include "UserManager.h"
#include "Product.h"

namespace Constants
{
	const size_t COMMAND_INDEX = 0;
};

class Program
{
public:
	Program();

	void run();

	~Program();

private:
	void loop();
	void save();

	UserManager userManager;
	Vector<Product*> products;
};