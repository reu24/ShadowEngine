#pragma once
#include <iostream>
#include <colorConsole/color.hpp>

namespace std
{
	inline std::string to_string(const std::string& str)
	{
		return str;
	}
}

/*
	Logs a white information text to the console. 
	Usage: If you want to inform something that maybe is important but probably not.
	@param info: The text that it should print.
	@param logLocation: Wheather it should log where you called this macro or not.
*/
#define LOG_INFO(info, logLocation) logInfo(std::to_string(info), __FILE__, __LINE__, logLocation)
/*
	Logs a yellow warning text to the console. 
	Useage: If you want to say that something shouldn't be done but everything still works.
	@param warning: The text that it should print.
	@param logLocation: Wheather it should log where you called this macro or not.
*/
#define LOG_WARNING(warning, logLocation) logWarning(std::to_string(warning), __FILE__, __LINE__, logLocation)
/*
	Logs a light red error text to the console.
	Usage: If some code can't run then use this macro.
	@param error: The text that it should print.
	@param logLocation: Wheather it should log where you called this macro or not.
*/
#define LOG_ERROR(error, logLocation) logError(std::to_string(error), __FILE__, __LINE__, logLocation)
/*
	Logs a dark red fatal text to the console.
	Usage: If a core function doesn't work and this will break everything then use this macro.
	@param fatal: The text that it should print.
	@param logLocation: Wheather it should log where you called this macro or not.
*/
#define LOG_FATAL(fatal, logLocation) logFatal(std::to_string(fatal), __FILE__, __LINE__, logLocation)

/*
	INTERNAL! DON'T USE!
	INSTEAD USE LOG_INFO!
*/
static void logInfo(std::string message, std::string file, int line, bool logLocation)
{
	if (logLocation)
	{
		std::cout << hue::white << file + "(" + std::to_string(line) + "):\n" + message + '\n' << hue::white;
	}
	else
	{
		std::cout << hue::white << message + '\n' << hue::white;
	}
}

/*
	INTERNAL! DON'T USE!
	INSTEAD USE LOG_WARNING!
*/
static void logWarning(std::string message, std::string file, int line, bool logLocation)
{
	if (logLocation)
	{
		std::cout << hue::yellow << file + "(" + std::to_string(line) + "):\n" + message + '\n' << hue::white;
	}
	else
	{
		std::cout << hue::yellow << message + '\n' << hue::white;
	}
}

/*
	INTERNAL! DON'T USE!
	INSTEAD USE LOG_ERROR!
*/
static void logError(std::string message, std::string file, int line, bool logLocation)
{
	if (logLocation)
	{
		std::cout << hue::light_red << file + "(" + std::to_string(line) + "):\n" + message + '\n' << hue::white;
	}
	else
	{
		std::cout << hue::light_red << message + '\n' << hue::white;
	}
}

/*
	INTERNAL! DON'T USE!
	INSTEAD USE LOG_FATAL!
*/
static void logFatal(std::string message, std::string file, int line, bool logLocation)
{
	if (logLocation)
	{
		std::cout << hue::red << file + "(" + std::to_string(line) + "):\n" + message + '\n' << hue::white;
	}
	else
	{
		std::cout << hue::red << message + '\n' << hue::white;
	}
}