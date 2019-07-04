#pragma once

#include <functional>
#include <string>
#include <cstring>

#include <esp_http_server.h>


namespace fc::web
{
    class websocket_server;
    namespace ws_def
    {
        typedef std::function<esp_err_t(websocket_server)> handler_func;

        typedef enum {
            CONTINUATION            = 0,
            TEXT                    = 0x1,
            BINARY                  = 0x2,
            CONNECTION_CLOSE        = 0x8,
            PING                    = 0x9,
            PONG                    = 0xA
        } opcode;

        typedef struct __attribute__((packed)) {
            bool fin:1;
            uint8_t reserved:3;
            opcode type:4;
            bool mask:1;
            uint8_t payload_len:7;
            uint8_t extra_len[8];
        } header;
    }

    class websocket_server
    {
        public:
            explicit websocket_server(httpd_handle_t _handle);
            esp_err_t add_handler(const std::string& url, ws_def::handler_func func);
            esp_err_t remove_handler(const std::string& url);
            esp_err_t receive(std::string& str);
            esp_err_t receive(std::vector<uint8_t>& bytes);
            esp_err_t transmit(const std::string& str);
            esp_err_t transmit(const std::vector<uint8_t>& bytes);

        private:
            httpd_handle_t httpd_handle;
    };

}
