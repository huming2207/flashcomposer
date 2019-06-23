#pragma once

#include <esp_err.h>
#include <esp_http_server.h>

namespace fc
{
    class rest_spi_handler
    {
        public:
            explicit rest_spi_handler(httpd_handle_t httpd_handle);
            esp_err_t probe_handler(httpd_req_t *req);
            esp_err_t read_handler(httpd_req_t *req);
            esp_err_t erase_handler(httpd_req_t *req);
            esp_err_t program_handler(httpd_req_t *req);

        private:
            httpd_uri_t probe_handle {
                "/api/v1/spi/flash/probe",
                HTTP_GET,
                [](httpd_req_t *req) -> esp_err_t {
                    auto *spi_handler = static_cast<rest_spi_handler*>(req->user_ctx);
                    return spi_handler->probe_handler(req);
                },
                this
            };
    };
}