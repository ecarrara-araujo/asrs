#include "stdio.h"
#include "tchar.h"
#include "Logging.h"

FILE * CLogging::_filep = NULL;

CLogging::CLogging(void)
{
	open();
}

void CLogging::open()
{
	close();
	_filep = fopen("robolog.txt", "a" );
}

CLogging::~CLogging(void)
{
	if (_filep)
	{
		fclose(_filep);
		_filep=0;
	}
}

void CLogging::print(std::string robo, std::string msg)
{
	if(!_filep) open();
	fwrite(robo.c_str(),1, robo.size(), _filep);
	//fwrite(
	fwrite(msg.c_str(),1, msg.size(), _filep);
	fflush(_filep);
}

void CLogging::close()
{
	if (_filep)
	{
		fclose(_filep);
		_filep=NULL;
	}
}
