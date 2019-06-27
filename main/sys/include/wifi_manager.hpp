#pragma once

#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <esp_err.h>

namespace fc::sys
{
    namespace sysdef
    {
        enum wifi_event_bit
        {
            WIFI_STA_CONNECTED                  = BIT0,
            WIFI_STA_CONNECTED_IPV6             = BIT1,
            WIFI_AP_DEVICE_CONNECT              = BIT2,
            WIFI_AP_DEVICE_DISCONNECT           = BIT3,
            WIFI_STA_CONNECT_RETRY_MAXIMUM      = BIT4
        };
    }

    class wifi_manager
    {
        private:
            wifi_manager();
            static uint8_t sta_retry_cnt;
            static uint8_t sta_max_retry;
            static EventGroupHandle_t wifi_event_group;
            static ip6_addr_t ip6_addr;
            static ip_addr_t ip_addr;

        public:
            static wifi_manager& get_instance()
            {
                static wifi_manager instance;
                return instance;
            };
            wifi_manager(wifi_manager const &) = delete;
            void operator=(wifi_manager const &) = delete;

            void set_sta_max_retry(uint8_t _sta_max_retry);
            esp_err_t start_ap();
            esp_err_t start_sta();
            esp_err_t set_ap_config(const std::string &ssid, const std::string &passwd, uint8_t channel = 3);
            esp_err_t set_sta_config(const std::string& ssid, const std::string& passwd);
            esp_err_t clear_wifi_config();

    };

}

