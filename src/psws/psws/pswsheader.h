// psws.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <streambuf>
#define FILE_READER(F, M) std::string((std::istreambuf_iterator<char>(std::ifstream(F, M | std::ios::in).rdbuf())), std::istreambuf_iterator<char>())
#define FILE_WRITER(D, S, F, M) std::ofstream(F, M | std::ios::out).write((D), (S))

template <typename ...Args>
__inline static 
std::string format_string(const char* format, Args... args) {
	char buffer[BUFSIZ] = { 0 };
	size_t newlen = snprintf(buffer, BUFSIZ, format, args...);
	if (newlen > BUFSIZ)
	{
		std::vector<char> newbuffer(newlen + 1);
		snprintf(newbuffer.data(), newlen, format, args...);
		return std::string(newbuffer.data());
	}
	return buffer;
}

//Header files for pswsclient
#include <oatpp-websocket/WebSocket.hpp>
#include <oatpp-websocket/Connector.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp-websocket/ConnectionHandler.hpp>
#include <oatpp-websocket/WebSocket.hpp>
#include <oatpp-websocket/AsyncConnectionHandler.hpp>
#include <oatpp-websocket/AsyncWebSocket.hpp>
//Header files for pswsserver
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/network/ConnectionHandler.hpp>
#include <oatpp-websocket/ConnectionHandler.hpp>
#include <oatpp-websocket/WebSocket.hpp>
#include <oatpp-websocket/Handshaker.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/AsyncHttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
