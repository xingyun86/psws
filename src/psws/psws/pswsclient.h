// psws.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.

const char* WSCLIENT_TAG = "ps_client_websocket_listener";

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class WSClientListener : public oatpp::websocket::WebSocket::Listener {
private:
    std::mutex& m_writeMutex;
    /**
     * Buffer for messages. Needed for multi-frame messages.
     */
    oatpp::data::stream::ChunkedBuffer m_messageBuffer;
public:

    WSClientListener(std::mutex& writeMutex)
        : m_writeMutex(writeMutex)
    {}

    /**
     * Called on "ping" frame.
     */
    void onPing(const WebSocket& socket, const oatpp::String& message) override {
        OATPP_LOGD(WSCLIENT_TAG, "onPing");
        std::lock_guard<std::mutex> lock(m_writeMutex);
        socket.sendPong(message);
    }

    /**
     * Called on "pong" frame
     */
    void onPong(const WebSocket& socket, const oatpp::String& message) override {
        OATPP_LOGD(WSCLIENT_TAG, "onPong");
    }

    /**
     * Called on "close" frame
     */
    void onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message) override {
        OATPP_LOGD(WSCLIENT_TAG, "onClose code=%d", code);
    }

    /**
     * Called on each message frame. After the last message will be called once-again with size == 0 to designate end of the message.
     */
    void readMessage(const WebSocket& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override {

        if (size == 0) { // message transfer finished

            auto wholeMessage = m_messageBuffer.toString();
            m_messageBuffer.clear();

            OATPP_LOGD(WSCLIENT_TAG, "on message received '%s'", wholeMessage->c_str());

            /* Send message in reply */
            //std::lock_guard<std::mutex> lock(m_writeMutex);
            //socket.sendOneFrameText( "Hello from oatpp!: " + wholeMessage);

        }
        else if (size > 0) { // message frame received
            m_messageBuffer.writeSimple(data, size);
        }

    }
};

    static bool finished = false;
class PSWSClient {
public:

    static void socketTask(const std::shared_ptr<oatpp::websocket::WebSocket>& websocket) {
        websocket->listen();
        OATPP_LOGD(WSCLIENT_TAG, "SOCKET CLOSED!!!");
        finished = true;
    }
private:
    void run() {
        try
        {
            OATPP_LOGI(WSCLIENT_TAG, "Application Started");

            auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ PSWSConfig::Inst()->config->host->c_str()/*"echo.websocket.org"*/, (*PSWSConfig::Inst()->config->port)/*80*/ });

            auto connector = oatpp::websocket::Connector::createShared(connectionProvider);

            auto connection = connector->connect("/" + oatpp::String(PSWSConfig::Inst()->config->path->c_str()));

            OATPP_LOGI(WSCLIENT_TAG, "Connected");

            auto socket = oatpp::websocket::WebSocket::createShared(connection, true /* maskOutgoingMessages must be true for clients */);

            std::mutex socketWriteMutex;

            socket->setListener(std::make_shared<WSClientListener>(socketWriteMutex));

            std::thread thread(PSWSClient::socketTask, socket);

            while (!finished) {
                {
                    OATPP_LOGD(WSCLIENT_TAG, "sending message...");
                    std::lock_guard<std::mutex> lock(socketWriteMutex);
                    socket->sendOneFrameText("hello");
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            thread.join();
        }
        catch (const std::exception&e)
        {
            std::cerr << e.what();
        }
    }
public:
    int Start(int argc, char ** argv) {
        oatpp::base::Environment::init();
        run();
        oatpp::base::Environment::destroy();
        return 0;
    }
};
