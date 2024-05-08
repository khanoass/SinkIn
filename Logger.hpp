#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#define LOG

namespace Logger
{
	static void log(const std::vector<std::string>& str)
	{
#ifdef LOG
		std::stringstream ss;
		for (const auto& s : str)
			ss << s;
		ss << "\n";
		std::cout << ss.str();
#endif
	}

	static std::string afterNumber(int n)
	{
		int f = n % 10;

		if (f == 0 || f > 3) return "th";
		if (f == 1) return "st";
		if (f == 2) return "nd";
		if (f == 3) return "rd";
	}
}