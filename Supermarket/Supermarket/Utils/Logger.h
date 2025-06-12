#pragma once

#include <fstream>

#include "../Utils/String.h"

class Logger
{
public:
	static void log(const String& message)
	{
		std::ofstream os("files/feed_log.txt", std::ios::app);
		if (os.is_open())
		{
			os << message << '\n';
		}
		os.close();
	}
};