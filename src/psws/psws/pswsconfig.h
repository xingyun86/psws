// pswsconfig.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include OATPP_CODEGEN_BEGIN(DTO) ///< Begin DTO codegen section

class Config : public oatpp::DTO {
	DTO_INIT(Config, DTO /* extends */)

	DTO_FIELD(Boolean, type, "type");
	DTO_FIELD(String, host, "host");
	DTO_FIELD(UInt16, port, "port");
	DTO_FIELD(String, path, "path");
};

#include OATPP_CODEGEN_END(DTO) ///< End DTO codegen section
class PSWSConfig {
public:
	Config::Wrapper config = {};
public:
	auto Init(const oatpp::String& fileName="config.json") {
		 return (config = oatpp::parser::json::mapping::ObjectMapper().readFromString<oatpp::Object<Config>>(oatpp::String(FILE_READER(fileName->std_str(), std::ios::binary).c_str()))), this;
	}
	bool Check()
	{
		return ((config->type != nullptr) && (config->host != nullptr) && (config->port != nullptr) && (config->path != nullptr));
	}
	void Print()
	{
		if (Check() == true)
		{
			std::cout << ((*config->type) ? "Server:ws://" : "Client:ws://") << config->host->c_str() << ":" << (*config->port) << "/" << config->path->c_str() << std::endl;
		}
	}
public:
	static PSWSConfig* Inst() {
		static PSWSConfig PSWSConfigInstance;
		return &PSWSConfigInstance;
	}
};
