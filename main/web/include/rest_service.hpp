#pragma once

#include <esp_http_server.h>

#define FC_WEB_MDNS_NAME    "FlashComposer Web Service"
#define FC_WEB_MDNS_HOST    "flashcomposer"

namespace fc::web
{
    class rest_service
    {
        public:
            rest_service();

        private:
            void init_mdns();
            void init_httpd();
            httpd_handle_t httpd_handle = nullptr;
    };
}