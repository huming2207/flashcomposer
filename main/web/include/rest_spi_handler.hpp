#pragma once

#include <esp_err.h>
#include <esp_http_server.h>

namespace fc::web
{
    class rest_spi_handler
    {
        public:
            explicit rest_spi_handler(httpd_handle_t handle);
            esp_err_t hook_handler();
            esp_err_t probe_handler(httpd_req_t *req);
            esp_err_t read_handler(httpd_req_t *req);
            esp_err_t erase_handler(httpd_req_t *req);
            esp_err_t program_handler(httpd_req_t *req);

        private:
            httpd_handle_t httpd_handle = nullptr;
            httpd_uri_t probe_handle {
                "/api/v1/spi/flash/probe",
                HTTP_GET,
                [](httpd_req_t *req) -> esp_err_t {
                    auto *spi_handler = static_cast<rest_spi_handler*>(req->user_ctx);
                    return spi_handler->probe_handler(req);
                },
                this
            };

            httpd_uri_t read_handle {
                    "/api/v1/spi/flash/read",
                    HTTP_GET,
                    [](httpd_req_t *req) -> esp_err_t {
                        auto *spi_handler = static_cast<rest_spi_handler*>(req->user_ctx);
                        return spi_handler->read_handler(req);
                    },
                    this
            };

            httpd_uri_t erase_handle {
                    "/api/v1/spi/flash/erase",
                    HTTP_GET,
                    [](httpd_req_t *req) -> esp_err_t {
                        auto *spi_handler = static_cast<rest_spi_handler*>(req->user_ctx);
                        return spi_handler->erase_handler(req);
                    },
                    this
            };

            httpd_uri_t program_handle {
                    "/api/v1/spi/flash/program",
                    HTTP_POST,
                    [](httpd_req_t *req) -> esp_err_t {
                        auto *spi_handler = static_cast<rest_spi_handler*>(req->user_ctx);
                        return spi_handler->program_handler(req);
                    },
                    this
            };
    };
}