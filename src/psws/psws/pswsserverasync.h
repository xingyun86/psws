// psws.h : Include file for standard system include files,
// or project specific include files.

#pragma once

// TODO: Reference additional headers your program requires here.

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- codegen begin

/**
 * Controller with WebSocket-connect endpoint.
 */
class MyControllerAsync : public oatpp::web::server::api::ApiController {
private:
	typedef MyControllerAsync __ControllerType;
private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandlerAsync, "websocket");
public:
	MyControllerAsync(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{}
public:

	ENDPOINT_ASYNC("GET", "/async", RootAsync) {

		ENDPOINT_ASYNC_INIT(RootAsync)

			const std::string pageTemplate =
			"<html lang='en'>"
			"<head>"
			"<meta charset=utf-8/>"
			"</head>"
			"<body>"
			"<p>Hello Multithreaded WebSocket Server!</p>"
			"<p>"
			"<code>"
			"websocket endpoint is: "
			+ format_string("ws://%s:%d/%s", PSWSConfig::Inst()->config->host->c_str(), (*PSWSConfig::Inst()->config->port), PSWSConfig::Inst()->config->path->c_str())
			+ "<br>"
			"</code>"
			"</p>"
			"</body>"
			"</html>";

		Action act() override {
			return _return(controller->createResponse(Status::CODE_200, pageTemplate.c_str()));
		}
	};

	ENDPOINT_ASYNC("GET", "wsasync", wsasync) {

		ENDPOINT_ASYNC_INIT(wsasync)

		Action act() override {
			auto response = oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(), controller->websocketConnectionHandlerAsync);
			return _return(response);
		}
	};
	// TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) //<-- codegen end

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class WSServerListenerAsync : public oatpp::websocket::AsyncWebSocket::Listener {
private:
	static constexpr const char* TAG = "ps_server_websocket_listener_async";
private:
	/**
	 * Buffer for messages. Needed for multi-frame messages.
	 */
	oatpp::data::stream::ChunkedBuffer m_messageBuffer;
public:

	/**
	 * Called on "ping" frame.
	 */
	CoroutineStarter onPing(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) override {
		OATPP_LOGD(TAG, "onPing");
		return socket->sendPongAsync(message);
	}

	/**
	 * Called on "pong" frame
	 */
	CoroutineStarter onPong(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) override {
		OATPP_LOGD(TAG, "onPong");
		return nullptr; // do nothing
	}

	/**
	 * Called on "close" frame
	 */
	CoroutineStarter onClose(const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const oatpp::String& message) override {
		OATPP_LOGD(TAG, "onClose code=%d", code);
		return nullptr; // do nothing
	}

	/**
	 * Called on each message frame. After the last message will be called once-again with size == 0 to designate end of the message.
	 */
	CoroutineStarter readMessage(const std::shared_ptr<AsyncWebSocket>& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override {

		if (size == 0) { // message transfer finished

			auto wholeMessage = m_messageBuffer.toString();
			m_messageBuffer.clear();

			OATPP_LOGD(TAG, "onMessage message='%s'", wholeMessage->c_str());

			/* Send message in reply */
			return socket->sendOneFrameTextAsync("Hello from oatpp!: " + wholeMessage);

		}
		else if (size > 0) { // message frame received
			m_messageBuffer.writeSimple(data, size);
		}

		return nullptr; // do nothing
	}
};

/**
 * Listener on new WebSocket connections.
 */
class WSInstanceListenerAsync : public oatpp::websocket::AsyncConnectionHandler::SocketInstanceListener {
private:
	static constexpr const char* TAG = "ps_server_websocket_instance_listener_async";
public:
	/**
	* Counter for connected clients.
	*/
	std::atomic<v_int32> SOCKETS_ASYNC = (0);
public:

	/**
	 *  Called when socket is created
	 */
	void onAfterCreate_NonBlocking(const std::shared_ptr<WSServerListenerAsync::AsyncWebSocket>& socket, const std::shared_ptr<const ParameterMap>& params) override {

		WSInstanceListenerAsync::Inst()->SOCKETS_ASYNC++;
		OATPP_LOGD(TAG, "New Incoming Connection. Connection count=%d", WSInstanceListenerAsync::Inst()->SOCKETS_ASYNC.load());

		/* In this particular case we create one WSListener per each connection */
		/* Which may be redundant in many cases */
		socket->setListener(std::make_shared<WSServerListenerAsync>());
	}

	/**
	 *  Called before socket instance is destroyed.
	 */
	void onBeforeDestroy_NonBlocking(const std::shared_ptr<WSServerListenerAsync::AsyncWebSocket>& socket) override {

		WSInstanceListenerAsync::Inst()->SOCKETS_ASYNC--;
		OATPP_LOGD(TAG, "Connection closed. Connection count=%d", WSInstanceListenerAsync::Inst()->SOCKETS_ASYNC.load());

	}
public:
	static WSInstanceListenerAsync* Inst() {
		static WSInstanceListenerAsync WSInstanceListenerAsyncInstance;
		return &WSInstanceListenerAsyncInstance;
	}
};

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponentAsync {
public:
	/**
	 * Create Async Executor
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
		return std::make_shared<oatpp::async::Executor>(
			4 /* Data-Processing threads */,
			1 /* I/O threads */,
			1 /* Timer threads */
			);
		}());
	/**
	 *  Create ConnectionProvider component which listens on the port
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
		return oatpp::network::tcp::server::ConnectionProvider::createShared({ PSWSConfig::Inst()->config->host->c_str(), (*PSWSConfig::Inst()->config->port), oatpp::network::Address::IP_4 });
		}());

	/**
	 *  Create Router component
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
		return oatpp::web::server::HttpRouter::createShared();
		}());

	/**
   *  Create http ConnectionHandler component which uses Router component to route requests
   */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)("http" /* qualifier */, [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
		return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
		}());

	/**
	 *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
		return oatpp::parser::json::mapping::ObjectMapper::createShared();
		}());

	/**
	 *  Create websocket connection handler
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandlerAsync)("websocket" /* qualifier */, [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
		auto connectionHandler = oatpp::websocket::AsyncConnectionHandler::createShared(executor);
		connectionHandler->setSocketInstanceListener(std::make_shared<WSInstanceListenerAsync>());
		return connectionHandler;
		}());
};
class PSWSServerAsync {
private:
	void run() {
		try {
			/* Register Components in scope of run() method */
			AppComponentAsync componentsAsync;

			/* Get router component */
			OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

			/* Create MyController and add all of its endpoints to router */
			auto myControllerAsync = std::make_shared<MyControllerAsync>();
			myControllerAsync->addEndpointsToRouter(router);

			/* Get connection handler component */
			OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler, "http");

			/* Get connection provider component */
			OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

			/* Create server which takes provided TCP connections and passes them to HTTP connection handler */
			oatpp::network::Server server(connectionProvider, connectionHandler);

			/* Priny info about server port */
			OATPP_LOGI("MyAppServerAsync", "Server running on port %s", connectionProvider->getProperty("port").getData());

			/* Run server */
			server.run();

		}
		catch (const std::exception& e)
		{
			std::cerr << e.what();
		}
	}
public:
	int Start(int argc, char** argv) {
		oatpp::base::Environment::init();
		run();
		oatpp::base::Environment::destroy();
		return 0;
	}
};