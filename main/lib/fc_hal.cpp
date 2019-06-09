#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "fc_hal.hpp"

#define TAG "fc_hal"

using namespace fc;

esp_err_t fc_hal::spi_write(const uint8_t *payload, size_t len)
{
    spi_transaction_t spi_tract;
    memset(&spi_tract, 0, sizeof(spi_tract));

    spi_tract.tx_buffer = payload;
    spi_tract.length = len;
    spi_tract.rxlength = 0;

    return spi_device_transmit(device_handle, &spi_tract);
}

esp_err_t fc_hal::spi_read(const uint8_t *tx_payload, size_t tx_len, uint8_t *rx_payload, size_t rx_len)
{
    spi_transaction_t spi_tract;
    memset(&spi_tract, 0, sizeof(spi_tract));

    spi_tract.tx_buffer = tx_payload;
    spi_tract.length = tx_len;
    spi_tract.rx_buffer = rx_payload;
    spi_tract.rxlength = rx_len;

    return spi_device_transmit(device_handle, &spi_tract);
}

fc_hal::fc_hal()
{
    ESP_LOGI(TAG, "Initialising GPIO...");
    ESP_LOGI(TAG, "Performing GPIO init...");
    ESP_ERROR_CHECK(gpio_set_direction((gpio_num_t)CONFIG_FC_IO_RST, GPIO_MODE_OUTPUT));
    ESP_LOGI(TAG, "GPIO initialization finished, resetting flash...");

    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t)CONFIG_FC_IO_RST, 0));
    vTaskDelay(pdMS_TO_TICKS(1));
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t)CONFIG_FC_IO_RST, 1));
    vTaskDelay(pdMS_TO_TICKS(10));

    spi_bus_config_t bus_config = {
            .mosi_io_num = CONFIG_FC_SPI_MOSI,
            .sclk_io_num = CONFIG_FC_SPI_SCLK,
            .miso_io_num = CONFIG_FC_SPI_MISO,
            .quadhd_io_num = -1,
            .quadwp_io_num = -1,
            .max_transfer_sz = 256 * 1024 * 1024 // Maybe??
    };

    spi_device_interface_config_t device_config = {
#ifndef CONFIG_FC_SPI_CLK_DEBUG
            .clock_speed_hz = SPI_MASTER_FREQ_40M,
#else
            .clock_speed_hz = SPI_MASTER_FREQ_8M,
#endif
            .mode = 0, // CPOL = 0, CPHA = 0???
            .spics_io_num = CONFIG_FC_SPI_CS,
            .queue_size = 7
    };

    ESP_LOGI(TAG, "Performing SPI init...");
    ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &bus_config, 1));
    ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &device_config, &device_handle));
    ESP_LOGI(TAG, "SPI initialization finished, ready to rock!");
}
