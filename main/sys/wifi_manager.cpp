#include <cstring>

#include <esp_system.h>
#include <esp_wifi.h>

#include "lwip/err.h"
#include "lwip/sys.h"

#include "wifi_manager.hpp"

#define TAG "wifi_mgr"

using namespace fc::sys;

esp_err_t wifi_manager::start_ap(const std::string &ssid, const std::string &passwd)
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, []
                    (void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
                       if (event_id == WIFI_EVENT_AP_STACONNECTED) {
                           auto* event = (wifi_event_ap_staconnected_t*) event_data;
                           ESP_LOGI(TAG, "Station " MACSTR " joined, AID=%d",
                                    MAC2STR(event->mac), event->aid);
                       } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
                           auto* event = (wifi_event_ap_stadisconnected_t*) event_data;
                           ESP_LOGI(TAG, "Station " MACSTR " left, AID=%d",
                                    MAC2STR(event->mac), event->aid);
                       }
                    },
                    nullptr));

    wifi_config_t wifi_config{};
    wifi_config.ap.authmode = passwd.empty() ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;
    wifi_config.ap.beacon_interval = 100;
    wifi_config.ap.channel = 3;
    std::strcpy(reinterpret_cast<char *>(wifi_config.sta.ssid), ssid.c_str());
    std::strcpy(reinterpret_cast<char *>(wifi_config.sta.password), passwd.c_str());
    wifi_config.ap.ssid_len = ssid.size();

    ESP_LOGI(TAG, "Starting AP...");

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "AP started!");

    return ESP_OK;
}

esp_err_t wifi_manager::start_sta(const std::string &ssid, const std::string &passwd)
{
    return 0;
}

void wifi_manager::wifi_nvs_preinit()
{
    auto ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
}
