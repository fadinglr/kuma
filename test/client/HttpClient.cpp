#include "HttpClient.h"

HttpClient::HttpClient(TestLoop* loop, long conn_id)
: loop_(loop)
, http_request_(loop->getEventLoop(), "HTTP/2.0")
, total_bytes_read_(0)
, conn_id_(conn_id)
{
    
}

void HttpClient::startRequest(std::string& url)
{
    http_request_.setDataCallback([this] (uint8_t* data, size_t len) { onData(data, len); });
    http_request_.setWriteCallback([this] (int err) { onSend(err); });
    http_request_.setErrorCallback([this] (int err) { onClose(err); });
    http_request_.setHeaderCompleteCallback([this] { onHeaderComplete(); });
    http_request_.setResponseCompleteCallback([this] { onRequestComplete(); });
    http_request_.sendRequest("GET", url.c_str());
}

int HttpClient::close()
{
    return http_request_.close();
}

void HttpClient::onData(uint8_t* data, size_t len)
{
    std::string str((char*)data, len);
    total_bytes_read_ += len;
    printf("HttpClient::onData, len=%zu, total=%u\n", len, total_bytes_read_);
}

void HttpClient::onSend(int err)
{
    
}

void HttpClient::onClose(int err)
{
    printf("HttpClient::onClose, err=%d\n", err);
    loop_->removeObject(conn_id_);
}

void HttpClient::onHeaderComplete()
{
    printf("HttpClient::onHeaderComplete\n");
}

void HttpClient::onRequestComplete()
{
    printf("HttpClient::onRequestComplete\n");
    http_request_.close();
}
