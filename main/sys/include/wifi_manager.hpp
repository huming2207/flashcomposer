#pragma once

#include <string>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <esp_err.h>

namespace fc::sys
{
    class wifi_manager
    {
        private:
            static void wifi_nvs_preinit();

        public:
            static esp_err_t start_ap(const std::string &ssid, const std::string &passwd);
            static esp_err_t start_sta(const std::string &ssid, const std::string &passwd);
    };

}

