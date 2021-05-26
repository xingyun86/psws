// psws.cpp : Defines the entry point for the application.
//

#include "psws.h"

int main(int argc, char ** argv)
{
	std::cout << "Hello CMake." << std::endl;
	if (PSWSConfig::Inst()->Init()->Check())
	{
		PSWSConfig::Inst()->Print();
		if ((*PSWSConfig::Inst()->config->type) == true) {
			PSWSServer().Start(argc, argv);
		}
		else
		{
			PSWSClient().Start(argc, argv);
		}
	}
	return 0;
}
