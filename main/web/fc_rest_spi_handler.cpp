#include <esp_system.h>
#include <esp_http_server.h>
#include <esp_log.h>

#include "fc_rest_spi_handler.hpp"

using namespace fc;

rest_spi_handler::rest_spi_handler(httpd_handle_t handle)
{
    assert(handle != nullptr);
    httpd_handle = handle;
}

esp_err_t rest_spi_handler::probe_handler(httpd_req_t *req)
{
    return ESP_OK;
}

esp_err_t rest_spi_handler::read_handler(httpd_req_t *req)
{
    return ESP_OK;
}

esp_err_t rest_spi_handler::erase_handler(httpd_req_t *req)
{
    return ESP_OK;
}

esp_err_t rest_spi_handler::program_handler(httpd_req_t *req)
{
    return ESP_OK;
}

esp_err_t rest_spi_handler::hook_handler()
{
    auto ret = httpd_register_uri_handler(httpd_handle, &probe_handle);
    ret = ret ?: httpd_register_uri_handler(httpd_handle, &read_handle);
    ret = ret ?: httpd_register_uri_handler(httpd_handle, &erase_handle);
    ret = ret ?: httpd_register_uri_handler(httpd_handle, &program_handle);
    return ret;
}

