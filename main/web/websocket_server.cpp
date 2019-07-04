#include <functional>
#include <string>
#include <cstring>

#include "websocket_server.hpp"

using namespace fc::web;

websocket_server::websocket_server(httpd_handle_t _handle)
{
    httpd_handle = _handle;
}

esp_err_t websocket_server::add_handler(const std::string &url, ws_def::handler_func func)
{
    return 0;
}

esp_err_t websocket_server::receive(std::string &str)
{
    return 0;
}

esp_err_t websocket_server::receive(std::vector<uint8_t> &bytes)
{
    return 0;
}

esp_err_t websocket_server::transmit(const std::string &str)
{
    return 0;
}

esp_err_t websocket_server::transmit(const std::vector<uint8_t> &bytes)
{
    return 0;
}

esp_err_t websocket_server::remove_handler(const std::string &url)
{
    return 0;
}
