#pragma once
#include <string>

class CLogging
{
	static FILE * _filep;
	CLogging();
public:
	~CLogging();
	static void open();
	static void close();
	static void print(std::string robo, std::string msg);
};
