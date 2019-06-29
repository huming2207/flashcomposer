#include <esp_system.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "fc_main.hpp"

#include "spi_flash.hpp"

#include <ArduinoJson.h>
#include <wifi_manager.hpp>

#define TAG "fc_main"

using namespace fc;

extern "C" {
    void app_main();
}

void app_main()
{
    fc_main main;
    main.start_main();
}

void fc_main::start_main()
{
//    hardware::spi_flash flash;
//    esp_err_t ret = flash.chip_reset();
//    ret = ret ?: flash.probe_flash();
//    while(ret != ESP_OK && ret != ESP_ERR_NOT_SUPPORTED) {
//        ESP_LOGI(TAG, "Failed to probe the flash, waiting for another 1 sec to retry...");
//        vTaskDelay(pdMS_TO_TICKS(1000));
//        ret = flash.probe_flash();
//    }
//
//    if(ret == ESP_ERR_NOT_SUPPORTED) {
//        ESP_LOGE(TAG, "This SPI flash is not supported!");
//    }
//
//    auto chip_info = flash.get_chip_info();
//
//    ESP_LOGI(TAG, "Detected %s %s, size %u KB",
//            chip_info.mf_name.c_str(), chip_info.chip_name.c_str(), chip_info.full_size_kb);

    sys::wifi_manager& wifi_man = sys::wifi_manager::get_instance();
    auto ret = wifi_man.set_ap_config("esp_test", "whatever123");
    ESP_LOGI(TAG, "Set config returns %u", ret);
    ret = ret ?: wifi_man.start_ap();
    ESP_LOGI(TAG, "Start AP returns %u", ret);
    vTaskDelay(portMAX_DELAY);
}
