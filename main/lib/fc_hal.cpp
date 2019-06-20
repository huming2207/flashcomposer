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
    spi_tract.length = len * 8;
    spi_tract.rxlength = 0;

    return spi_device_polling_transmit(device_handle, &spi_tract);
}

esp_err_t fc_hal::spi_write(const uint8_t cmd)
{
    return spi_write(&cmd, 1);
}

esp_err_t fc_hal::spi_write(uint8_t cmd, uint32_t addr, const uint8_t *tx_payload, size_t tx_len, bool is_4ba)
{
    // Tx length must be less than or equals 2
    if(tx_len > 2) return ESP_ERR_INVALID_ARG;
    if(is_4ba) {
        uint8_t buf[7] = { cmd, (uint8_t)(addr >> 24u), (uint8_t)(addr >> 16u),
                           (uint8_t)(addr >> 8u), (uint8_t)(addr & 0xffU), 0, 0 };

        for(uint8_t idx = 0; idx < tx_len; idx++) {
            buf[5 + idx] = tx_payload[idx]; // Append two Tx payload, if exists
        }

        return spi_write(buf, 5 + tx_len);
    } else {
        if(addr > 0xfff) return ESP_ERR_INVALID_STATE;
        uint8_t buf[6] = { cmd, (uint8_t)(addr >> 16u), (uint8_t)(addr >> 8u), (uint8_t)(addr & 0xffU), 0, 0 };

        for(uint8_t idx = 0; idx < tx_len; idx++) {
            buf[4 + idx] = tx_payload[idx]; // Append two Tx payload, if exists
        }

        return spi_write(buf, 4 + tx_len);
    }
}

esp_err_t fc_hal::spi_read(const uint8_t *tx_payload, size_t tx_len, uint8_t *rx_payload, size_t rx_len)
{
    spi_transaction_t spi_tract;
    memset(&spi_tract, 0, sizeof(spi_tract));

    spi_tract.tx_buffer = tx_payload;
    spi_tract.length = tx_len * 8;
    spi_tract.rx_buffer = rx_payload;
    spi_tract.rxlength = rx_len * 8;

    return spi_device_polling_transmit(device_handle, &spi_tract);
}

esp_err_t fc_hal::spi_read(const uint8_t reg, uint8_t *rx_payload, size_t rx_len)
{
    return spi_read(&reg, 1, rx_payload, rx_len);
}

esp_err_t fc_hal::spi_read(uint8_t cmd, uint32_t addr, const uint8_t *tx_payload, size_t tx_len,
                            uint8_t *rx_payload, size_t rx_len, bool is_4ba)
{
    // Tx length must be less than or equals 2
    if(tx_len > 2) return ESP_ERR_INVALID_ARG;
    if(is_4ba) {
        uint8_t buf[7] = { cmd, (uint8_t)(addr >> 24u), (uint8_t)(addr >> 16u),
                           (uint8_t)(addr >> 8u), (uint8_t)(addr & 0xffU), 0, 0 };

        for(uint8_t idx = 0; idx < tx_len; idx++) {
            buf[5 + idx] = tx_payload[idx]; // Append two Tx payload, if exists
        }

        return spi_read(buf, 5 + tx_len, rx_payload, rx_len);
    } else {
        if(addr > 0xfff) return ESP_ERR_INVALID_STATE;
        uint8_t buf[6] = { cmd, (uint8_t)(addr >> 16u), (uint8_t)(addr >> 8u), (uint8_t)(addr & 0xffU), 0, 0 };

        for(uint8_t idx = 0; idx < tx_len; idx++) {
            buf[4 + idx] = tx_payload[idx]; // Append two Tx payload, if exists
        }

        return spi_read(buf, 4 + tx_len, rx_payload, rx_len);
    }
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

    spi_bus_config_t bus_config{};
    memset(&bus_config, 0, sizeof(spi_bus_config_t));
    bus_config.mosi_io_num = CONFIG_FC_SPI_MOSI;
    bus_config.sclk_io_num = CONFIG_FC_SPI_SCLK;
    bus_config.miso_io_num = CONFIG_FC_SPI_MISO;
    bus_config.quadhd_io_num = -1;
    bus_config.quadwp_io_num = -1;
    bus_config.flags = 0;
    bus_config.max_transfer_sz = 2 * 1024 * 1024; // Maybe 2MB is enough?

    spi_device_interface_config_t device_config{};
    memset(&device_config, 0, sizeof(spi_device_interface_config_t));
#ifndef CONFIG_FC_SPI_CLK_DEBUG
    device_config.clock_speed_hz = SPI_MASTER_FREQ_40M;
#else
    device_config.clock_speed_hz = SPI_MASTER_FREQ_8M;
#endif
    device_config.mode = 0; // CPOL = 0, CPHA = 0???
    device_config.spics_io_num = CONFIG_FC_SPI_CS;
    device_config.flags = SPI_DEVICE_HALFDUPLEX; // Must use Half Duplex mode, or Rx length must be <= Tx length
    device_config.queue_size = 7;

    ESP_LOGI(TAG, "Performing SPI init...");
    ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &bus_config, 0));
    ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &device_config, &device_handle));
    ESP_LOGI(TAG, "SPI initialization finished, ready to rock!");
}

