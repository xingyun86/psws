// psws.cpp : Defines the entry point for the application.
//

#include "psws.h"

int main(int argc, char ** argv)
{
	/* set lockingCallback for libressl */
	oatpp::libressl::Callbacks::setDefaultCallbacks();
	/*{
		PSHttpClientAsync httpClient = {};
		httpClient.Start(argc, argv);
		return 0;
	}*/
	{
		PSHttpClient httpClient = {};
		httpClient.Start(argc, argv);
		return 0;
	}
	std::cout << "Hello CMake." << std::endl;
	if (PSWSConfig::Inst()->Init()->Check())
	{
		PSWSConfig::Inst()->Print();
		if ((*PSWSConfig::Inst()->config->type) == true) {
			PSWSServerAsync().Start(argc, argv);
			//PSWSServer().Start(argc, argv);
		}
		else
		{
			PSWSClient().Start(argc, argv);
		}
	}
	return 0;
}
