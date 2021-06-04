// psws.h : Include file for standard system include files,
// or project specific include files.

#pragma once

// TODO: Reference additional headers your program requires here.

const char* WSSERVER_TAG = "ps_server_websocket_listener";

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- codegen begin

/**
 * Controller with WebSocket-connect endpoint.
 */
class MyController : public oatpp::web::server::api::ApiController {
private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandler, "websocket");
public:
	MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{}
public:

	ENDPOINT("GET", "/", root) {
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

		return createResponse(Status::CODE_200, pageTemplate.c_str());

	};

	/*ENDPOINT("GET", "ws", ws, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
		return oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(), websocketConnectionHandler);
	};*/
	ENDPOINT("GET", PSWSConfig::Inst()->config->path->c_str(), ws, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
		return oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(), websocketConnectionHandler);
	};

	// TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) //<-- codegen end

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class WSServerListener : public oatpp::websocket::WebSocket::Listener {
private:
	/**
	 * Buffer for messages. Needed for multi-frame messages.
	 */
	oatpp::data::stream::ChunkedBuffer m_messageBuffer;
public:

	/**
	 * Called on "ping" frame.
	 */
	void onPing(const WebSocket& socket, const oatpp::String& message) override {
		OATPP_LOGD(WSSERVER_TAG, "onPing");
		socket.sendPong(message);
	}

	/**
	 * Called on "pong" frame
	 */
	void onPong(const WebSocket& socket, const oatpp::String& message) override {
		OATPP_LOGD(WSSERVER_TAG, "onPong");
	}

	/**
	 * Called on "close" frame
	 */
	void onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message) override {
		OATPP_LOGD(WSSERVER_TAG, "onClose code=%d", code);
	}

	/**
	 * Called on each message frame. After the last message will be called once-again with size == 0 to designate end of the message.
	 */
	void readMessage(const WebSocket& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override {

		if (size == 0) { // message transfer finished

			auto wholeMessage = m_messageBuffer.toString();
			m_messageBuffer.clear();

			OATPP_LOGD(WSSERVER_TAG, "onMessage message='%s'", wholeMessage->c_str());

			/* Send message in reply */
			socket.sendOneFrameText("Hello!: " + wholeMessage);

		}
		else if (size > 0) { // message frame received
			m_messageBuffer.writeSimple(data, size);
		}

	}
};

/**
 * Listener on new WebSocket connections.
 */
class WSInstanceListener : public oatpp::websocket::ConnectionHandler::SocketInstanceListener {
public:
	/**
	 * Counter for connected clients.
	 */
	std::atomic<v_int32> SOCKETS = 0;
public:

	/**
	 *  Called when socket is created
	 */
	void onAfterCreate(const oatpp::websocket::WebSocket& socket, const std::shared_ptr<const ParameterMap>& params) override {

		WSInstanceListener::Inst()->SOCKETS++;
		OATPP_LOGD(WSSERVER_TAG, "New Incoming Connection. Connection count=%d", WSInstanceListener::Inst()->SOCKETS.load());

		/* In this particular case we create one WSListener per each connection */
		/* Which may be redundant in many cases */
		socket.setListener(std::make_shared<WSServerListener>());
	}

	/**
	 *  Called before socket instance is destroyed.
	 */
	void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override {

		WSInstanceListener::Inst()->SOCKETS--;
		OATPP_LOGD(WSSERVER_TAG, "Connection closed. Connection count=%d", WSInstanceListener::Inst()->SOCKETS.load());
	}
public:
	static WSInstanceListener* Inst() {
		static WSInstanceListener WSInstanceListenerInstance;
		return &WSInstanceListenerInstance;
	}
};

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

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
	 *  Create http ConnectionHandler
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, httpConnectionHandler)("http" /* qualifier */, [] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
		return oatpp::web::server::HttpConnectionHandler::createShared(router);
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
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandler)("websocket" /* qualifier */, [] {
		auto connectionHandler = oatpp::websocket::ConnectionHandler::createShared();
		connectionHandler->setSocketInstanceListener(std::make_shared<WSInstanceListener>());
		return connectionHandler;
		}());

};
class PSWSServer {
private:
	void run() {
		try {
			/* Register Components in scope of run() method */
			AppComponent components;

			/* Get router component */
			OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

			/* Create MyController and add all of its endpoints to router */
			auto myController = std::make_shared<MyController>();
			myController->addEndpointsToRouter(router);

			/* Get connection handler component */
			OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler, "http");

			/* Get connection provider component */
			OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

			/* Create server which takes provided TCP connections and passes them to HTTP connection handler */
			oatpp::network::Server server(connectionProvider, connectionHandler);

			/* Priny info about server port */
			OATPP_LOGI(WSSERVER_TAG, "Server running on port %s", connectionProvider->getProperty("port").getData());

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