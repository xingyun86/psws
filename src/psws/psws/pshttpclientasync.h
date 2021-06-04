// psws.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.

const char* HTTPCLIENTAYYNC_TAG = "HttpClientAsync";

class HttpApiClientAsync : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)

    API_CLIENT_INIT(HttpApiClientAsync)

        //-----------------------------------------------------------------------------------------------
        // Asynchronous calls
        API_CALL_ASYNC("GET", "{path}", doGetAsync, PATH(String, path), BODY_STRING(String, body))
        API_CALL_ASYNC("POST", "{path}", doPostAsync, PATH(String, path), BODY_STRING(String, body))
        API_CALL_ASYNC("PUT", "{path}", doPutAsync, PATH(String, path), BODY_STRING(String, body))
        API_CALL_ASYNC("PATCH", "{path}", doPatchAsync, PATH(String, path), BODY_STRING(String, body))
        API_CALL_ASYNC("DELETE", "{path}", doDeleteAsync, PATH(String, path), BODY_STRING(String, body))

#include OATPP_CODEGEN_END(ApiClient)
};

class GetCoroutine : public oatpp::async::Coroutine<GetCoroutine> {
private:
    std::shared_ptr<HttpApiClientAsync> m_clientasync;
    oatpp::String m_path;
    oatpp::String m_body;
public:

    GetCoroutine(const std::shared_ptr<HttpApiClientAsync>& client, const oatpp::String& path, const oatpp::String& body)
        : m_clientasync(client), m_path(path), m_body(body) {}

    Action act() override {
        return m_clientasync->doGetAsync(m_path, m_body).callbackTo(&GetCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<oatpp::web::protocol::http::incoming::Response>& response) {
        return response->readBodyToStringAsync().callbackTo(&GetCoroutine::onBody);
    }

    Action onBody(const oatpp::String& body) {
        OATPP_LOGD(HTTPCLIENTAYYNC_TAG, "[GetCoroutine. doGetAsync] data='%s'", body->c_str());
        return finish();
    }
};
class PostCoroutine : public oatpp::async::Coroutine<PostCoroutine> {
private:
    std::shared_ptr<HttpApiClientAsync> m_clientasync;
    oatpp::String m_path;
    oatpp::String m_body;
public:

    PostCoroutine(const std::shared_ptr<HttpApiClientAsync>& client, const oatpp::String& path, const oatpp::String& body)
        : m_clientasync(client), m_path(path), m_body(body) {}

    Action act() override {
        return m_clientasync->doPostAsync(m_path, m_body).callbackTo(&PostCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<oatpp::web::protocol::http::incoming::Response>& response) {
        return response->readBodyToStringAsync().callbackTo(&PostCoroutine::onBody);
    }

    Action onBody(const oatpp::String& body) {
        OATPP_LOGD(HTTPCLIENTAYYNC_TAG, "[PostCoroutine. doPostAsync] data='%s'", body->c_str());
        return finish();
    }
};
class PutCoroutine : public oatpp::async::Coroutine<PutCoroutine> {
private:
    std::shared_ptr<HttpApiClientAsync> m_clientasync;
    oatpp::String m_path;
    oatpp::String m_body;
public:

    PutCoroutine(const std::shared_ptr<HttpApiClientAsync>& client, const oatpp::String& path, const oatpp::String& body)
        : m_clientasync(client), m_path(path), m_body(body) {}

    Action act() override {
        return m_clientasync->doPutAsync(m_path, m_body).callbackTo(&PutCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<oatpp::web::protocol::http::incoming::Response>& response) {
        return response->readBodyToStringAsync().callbackTo(&PutCoroutine::onBody);
    }

    Action onBody(const oatpp::String& body) {
        OATPP_LOGD(HTTPCLIENTAYYNC_TAG, "[PutCoroutine. doPutAsync] data='%s'", body->c_str());
        return finish();
    }
};
class PatchCoroutine : public oatpp::async::Coroutine<PatchCoroutine> {
private:
    std::shared_ptr<HttpApiClientAsync> m_clientasync;
    oatpp::String m_path;
    oatpp::String m_body;
public:

    PatchCoroutine(const std::shared_ptr<HttpApiClientAsync>& client, const oatpp::String& path, const oatpp::String& body)
        : m_clientasync(client), m_path(path), m_body(body) {}

    Action act() override {
        return m_clientasync->doPatchAsync(m_path, m_body).callbackTo(&PatchCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<oatpp::web::protocol::http::incoming::Response>& response) {
        return response->readBodyToStringAsync().callbackTo(&PatchCoroutine::onBody);
    }

    Action onBody(const oatpp::String& body) {
        OATPP_LOGD(HTTPCLIENTAYYNC_TAG, "[PatchCoroutine. doPatchAsync] data='%s'", body->c_str());
        return finish();
    }
};
class DeleteCoroutine : public oatpp::async::Coroutine<DeleteCoroutine> {
private:
    std::shared_ptr<HttpApiClientAsync> m_clientasync;
    oatpp::String m_path;
    oatpp::String m_body;
public:

    DeleteCoroutine(const std::shared_ptr<HttpApiClientAsync>& client, const oatpp::String& path, const oatpp::String& body)
        : m_clientasync(client), m_path(path), m_body(body) {}

    Action act() override {
        return m_clientasync->doDeleteAsync(m_path, m_body).callbackTo(&DeleteCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<oatpp::web::protocol::http::incoming::Response>& response) {
        return response->readBodyToStringAsync().callbackTo(&DeleteCoroutine::onBody);
    }

    Action onBody(const oatpp::String& body) {
        OATPP_LOGD(HTTPCLIENTAYYNC_TAG, "[DeleteCoroutine. doDeleteAsync] data='%s'", body->c_str());
        return finish();
    }
};
class HttpClientAsync {
private:
    std::shared_ptr<oatpp::async::Executor> executor = nullptr;
    std::shared_ptr<HttpApiClientAsync> m_clientasync = nullptr;
public:
    HttpClientAsync(const std::shared_ptr<HttpApiClientAsync>& clientasync) :m_clientasync(clientasync) {
        executor = std::make_shared<oatpp::async::Executor>(
            1 /* Data-Processing threads */,
            1 /* I/O threads */,
            1 /* Timer threads */);
    }
    virtual ~HttpClientAsync() {
        if (executor != nullptr) {
            executor->stop(); executor->join(); executor = nullptr;
        }
    }
public:

    void Get(const oatpp::String& path, const oatpp::String& body) {
        executor->execute<GetCoroutine>(m_clientasync, path, body);
        executor->waitTasksFinished();
    }
    void Post(const oatpp::String& path, const oatpp::String& body) {
        executor->execute<PostCoroutine>(m_clientasync, path, body);
        executor->waitTasksFinished();
    }
    void Put(const oatpp::String& path, const oatpp::String& body) {
        executor->execute<PutCoroutine>(m_clientasync, path, body);
        executor->waitTasksFinished();
    }
    void Patch(const oatpp::String& path, const oatpp::String& body) {
        executor->execute<PatchCoroutine>(m_clientasync, path, body);
        executor->waitTasksFinished();
    }
    void Delete(const oatpp::String& path, const oatpp::String& body) {
        executor->execute<DeleteCoroutine>(m_clientasync, path, body);
        executor->waitTasksFinished();
    }
};

class PSHttpClientAsync {
private:

    std::shared_ptr<oatpp::web::client::RequestExecutor> createOatppExecutor(const char* host, uint16_t port, bool ssl) {
        OATPP_LOGD(HTTPCLIENTAYYNC_TAG, "Using native HttpRequestExecutor.");
        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ host, port, oatpp::network::Address::IP_4 });
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
                return HttpApiClientAsync::createShared(createOatppExecutorLibressl(oatpp::String(url->c_str() + 8, j - 8, true)->c_str(), 443, true), objectMapper); // <-- Always use oatpp native executor where's possible.
            }
            else if (i != std::string::npos && i < j)
            {
                bool success = false;
                uint16_t port = oatpp::utils::conversion::strToInt32(oatpp::String(url->c_str() + i + 1, j - i - 1, true), success);
                if (success == true)
                {
                    return HttpApiClientAsync::createShared(createOatppExecutorLibressl(oatpp::String(url->c_str() + 8, i - 8, true)->c_str(), port, true), objectMapper); // <-- Always use oatpp native executor where's possible.
                }
            }
        }
        else //if (url->startsWith("http://"))
        {
            auto i = url->std_str().find(':', 7);
            auto j = url->std_str().find('/', 7);
            if (i == std::string::npos)
            {
                return HttpApiClientAsync::createShared(createOatppExecutor(oatpp::String(url->c_str() + 7, j - 7, true)->c_str(), 80, false), objectMapper); // <-- Always use oatpp native executor where's possible.
            }
            else if (i != std::string::npos && i < j)
            {
                bool success = false;
                uint16_t port = oatpp::utils::conversion::strToInt32(oatpp::String(url->c_str() + i + 1, j - i - 1, true), success);
                if (success == true)
                {
                    return HttpApiClientAsync::createShared(createOatppExecutor(oatpp::String(url->c_str() + 7, i - 7, true)->c_str(), port, false), objectMapper); // <-- Always use oatpp native executor where's possible.
                }
            }
        }
    }
    void run() {
        try
        {
            OATPP_LOGI(HTTPCLIENTAYYNC_TAG, "Application Started");

            /* Create ObjectMapper for serialization of DTOs  */
            auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

            auto clientasync = Request("http://www.ppsbbs.tech/","", objectMapper);
            //auto clientasync = Request("https://127.0.0.1:8080/","", objectMapper);
            HttpClientAsync httpClientAsync(clientasync);
            httpClientAsync.Get("my-notice.htm","");
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
