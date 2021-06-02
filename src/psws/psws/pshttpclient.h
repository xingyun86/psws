// psws.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.

class HttpApiClient : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)

    API_CLIENT_INIT(HttpApiClient)

        //-----------------------------------------------------------------------------------------------
        // Synchronous calls
        API_CALL("GET", "{path}", doGet, PATH(String, path), BODY_STRING(String, body))
        API_CALL("POST", "{path}", doPost, PATH(String, path), BODY_STRING(String, body))
        API_CALL("PUT", "{path}", doPut, PATH(String, path), BODY_STRING(String, body))
        API_CALL("PATCH", "{path}", doPatch, PATH(String, path), BODY_STRING(String, body))
        API_CALL("DELETE", "{path}", doDelete, PATH(String, path), BODY_STRING(String, body))

#include OATPP_CODEGEN_END(ApiClient)
};

class PSHttpClient {
private:
    std::shared_ptr<oatpp::web::client::RequestExecutor> createOatppExecutor() {
        OATPP_LOGD("App", "Using Oat++ native HttpRequestExecutor.");
        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ "httpbin.org", 80, oatpp::network::Address::IP_4 });
        return oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
    }
    std::shared_ptr<oatpp::web::client::RequestExecutor> createOatppExecutorLibressl(const char* host, uint16_t port, bool ssl) {
        if (!ssl)
        {
            auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ host, port, oatpp::network::Address::IP_4 });
            return oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);            
        }
        auto config = oatpp::libressl::Config::createShared();
        tls_config_insecure_noverifycert(config->getTLSConfig());
        tls_config_insecure_noverifyname(config->getTLSConfig());
        auto connectionProvider = oatpp::libressl::client::ConnectionProvider::createShared(config, { host, port, oatpp::network::Address::IP_4 });
        return oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
    }
    auto Request(const oatpp::String& url, const oatpp::String& data, const std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper>& objectMapper)
    {
        if (url->startsWith("https://"))
        {
            auto i = url->std_str().find(':', 8);
            auto j = url->std_str().find('/', 8);
            if (i == std::string::npos)
            {
                /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
                /* ObjectMapper passed here is used for serialization of outgoing DTOs */
                return HttpApiClient::createShared(createOatppExecutorLibressl(oatpp::String(url->c_str() + 8, j - 8, true)->c_str(), 443, true), objectMapper); // <-- Always use oatpp native executor where's possible.
            }
            else if (i != std::string::npos && i < j)
            {
                bool success = false;
                uint16_t port = oatpp::utils::conversion::strToInt32(oatpp::String(url->c_str() + i + 1, j - i - 1, true), success);
                if (success == true)
                {
                    /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
                    /* ObjectMapper passed here is used for serialization of outgoing DTOs */
                    return HttpApiClient::createShared(createOatppExecutorLibressl(oatpp::String(url->c_str() + 8, i - 8, true)->c_str(), port, true), objectMapper); // <-- Always use oatpp native executor where's possible.
                }
            }
        }
        else //if (url->startsWith("http://"))
        {
            auto i = url->std_str().find(':', 7);
            auto j = url->std_str().find('/', 7);
            if (i == std::string::npos)
            {
                /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
                /* ObjectMapper passed here is used for serialization of outgoing DTOs */
                return HttpApiClient::createShared(createOatppExecutorLibressl(oatpp::String(url->c_str() + 7, j - 7, true)->c_str(), 80, false), objectMapper); // <-- Always use oatpp native executor where's possible.
            }
            else if (i != std::string::npos && i < j)
            {
                bool success = false;
                uint16_t port = oatpp::utils::conversion::strToInt32(oatpp::String(url->c_str() + i + 1, j - i - 1, true), success);
                if (success == true)
                {
                    /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
                    /* ObjectMapper passed here is used for serialization of outgoing DTOs */
                    return HttpApiClient::createShared(createOatppExecutorLibressl(oatpp::String(url->c_str() + 7, i - 7, true)->c_str(), port, false), objectMapper); // <-- Always use oatpp native executor where's possible.
                }
            }
        }
    }
    void run() {
        try
        {
            OATPP_LOGI(TAG, "Application Started");

            /* Create ObjectMapper for serialization of DTOs  */
            auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

            //auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ PSWSConfig::Inst()->config->host->c_str()/*"echo.websocket.org"*/, (*PSWSConfig::Inst()->config->port)/*80*/ });
           
            /* Create RequestExecutor which will execute ApiClient's requests */
            //auto requestExecutor = createOatppExecutor();   // <-- Always use oatpp native executor where's possible.
            /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
            /* ObjectMapper passed here is used for serialization of outgoing DTOs */
            //auto client = HttpApiClient::createShared(requestExecutor, objectMapper);
            {
                auto client = Request("https://httpbin.org/", "", objectMapper);
                {
                    auto data = client->doGet("anything/a", "")->readBodyToString();
                    OATPP_LOGD(TAG, "[doGet] data='%s'", data->c_str());
                }

                {
                    auto data = client->doPost("anything/b", "aaa")->readBodyToString();
                    OATPP_LOGD(TAG, "[doPost] data='%s'", data->c_str());
                }
            }
            {
                auto client = Request("https://www.baidu.com/", "", objectMapper);
                {
                    auto data = client->doGet("", "")->readBodyToString();
                    OATPP_LOGD(TAG, "[doGet] data='%s'", data->c_str());
                }

                {
                    auto data = client->doPost("anything/b", "aaa")->readBodyToString();
                    OATPP_LOGD(TAG, "[doPost] data='%s'", data->c_str());
                }
            }
            {
                auto client = Request("http://127.0.0.1:9123/", "", objectMapper);
                {
                    auto data = client->doGet("json", "")->readBodyToString();
                    OATPP_LOGD(TAG, "[doGet] data='%s'", data->c_str());
                }

                {
                    auto data = client->doPost("anything/b", "aaa")->readBodyToString();
                    OATPP_LOGD(TAG, "[doPost] data='%s'", data->c_str());
                }
            }

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
