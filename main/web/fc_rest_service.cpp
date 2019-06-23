#include <esp_log.h>
#include <mdns.h>
#include "fc_rest_service.hpp"

#define TAG "rest_service"
using namespace fc;

rest_service::rest_service()
{
    init_mdns();
}

void rest_service::init_mdns()
{
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(FC_WEB_MDNS_HOST));
    ESP_ERROR_CHECK(mdns_service_instance_name_set("_http", "_tcp", FC_WEB_MDNS_NAME));
    ESP_ERROR_CHECK(mdns_service_add(nullptr, "_http", "_tcp", 80, nullptr, 0));
}

void rest_service::init_httpd()
{
    ESP_LOGI(TAG, "Starting web server...");
    httpd_config_t httpd_config{};
    httpd_config.backlog_conn = 8;
    httpd_config.close_fn = nullptr;
    httpd_config.core_id = tskNO_AFFINITY;
    httpd_config.ctrl_port = 32768;
    httpd_config.global_transport_ctx = nullptr;
    httpd_config.global_transport_ctx_free_fn = nullptr;
    httpd_config.global_user_ctx = nullptr;
    httpd_config.global_user_ctx_free_fn = nullptr;
    httpd_config.lru_purge_enable = false;
    httpd_config.max_open_sockets = 7;
    httpd_config.max_resp_headers = 8;
    httpd_config.max_uri_handlers = 20;
    httpd_config.open_fn = nullptr;
    httpd_config.recv_wait_timeout = 10;
    httpd_config.send_wait_timeout = 10;
    httpd_config.server_port = 80;
    httpd_config.stack_size = 10240;
    httpd_config.task_priority = tskIDLE_PRIORITY + 5;
    httpd_config.uri_match_fn = nullptr;

    ESP_ERROR_CHECK(httpd_start(&httpd_handle, &httpd_config));

    ESP_LOGI(TAG, "Adding handlers...");
    auto func = [](httpd_req_t req) -> esp_err_t {
        return ESP_OK;
    };
}
