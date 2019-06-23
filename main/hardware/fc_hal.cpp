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

    return spi_device_transmit(spi_dev_handle, &spi_tract);
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

    return spi_device_transmit(spi_dev_handle, &spi_tract);
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

esp_err_t fc_hal::spi_set_full_duplex()
{
    ESP_LOGI(TAG, "Freeing SPI device...");
    auto ret = spi_bus_remove_device(spi_dev_handle);
    if(ret != ESP_OK) {
        ESP_LOGW(TAG, "Current device is still busy, or something else went wrong!");
        return ret;
    }

    ESP_LOGI(TAG, "Freeing SPI bus...");
    ret = spi_bus_free(VSPI_HOST);
    if(ret != ESP_OK) {
        ESP_LOGW(TAG, "Current device is still busy, or something else went wrong!");
        return ret;
    }

    // The set-up procedure should never fail. If it fails, crash the whole program.
    ESP_LOGI(TAG, "Setting up full-duplex SPI bus and device...");
    ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &spi_bus_config, 1));
    ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &spi_dev_conf_fd, &spi_dev_handle));
    ESP_LOGI(TAG, "Full-duplex SPI has been set!");
    return ESP_OK;
}

esp_err_t fc_hal::spi_set_half_duplex()
{
    ESP_LOGI(TAG, "Freeing SPI device...");
    auto ret = spi_bus_remove_device(spi_dev_handle);
    if(ret != ESP_OK) {
        ESP_LOGW(TAG, "Current device is still busy, or something else went wrong!");
        return ret;
    }

    ESP_LOGI(TAG, "Freeing SPI bus...");
    ret = spi_bus_free(VSPI_HOST);
    if(ret != ESP_OK) {
        ESP_LOGW(TAG, "Current device is still busy, or something else went wrong!");
        return ret;
    }

    // The set-up procedure should never fail. If it fails, crash the whole program.
    ESP_LOGI(TAG, "Setting up half-duplex SPI bus and device...");
    ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &spi_bus_config, 0));
    ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &spi_dev_conf_hd, &spi_dev_handle));
    ESP_LOGI(TAG, "Half-duplex SPI has been set!");
    return ESP_OK;
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

    // Initialise SPI bus config
    memset(&spi_bus_config, 0, sizeof(spi_bus_config_t));
    spi_bus_config.mosi_io_num = CONFIG_FC_SPI_MOSI;
    spi_bus_config.sclk_io_num = CONFIG_FC_SPI_SCLK;
    spi_bus_config.miso_io_num = CONFIG_FC_SPI_MISO;
    spi_bus_config.quadhd_io_num = -1;
    spi_bus_config.quadwp_io_num = -1;
    spi_bus_config.flags = 0;
    spi_bus_config.max_transfer_sz = 2 * 1024 * 1024; // Maybe 2MB is enough?

    // Half duplex device config
    memset(&spi_dev_conf_hd, 0, sizeof(spi_device_interface_config_t));
#ifndef CONFIG_FC_SPI_CLK_DEBUG
    spi_dev_conf_hd.clock_speed_hz = SPI_MASTER_FREQ_40M;
#else
    spi_dev_conf_hd.clock_speed_hz = SPI_MASTER_FREQ_8M;
#endif
    spi_dev_conf_hd.mode = 0; // CPOL = 0, CPHA = 0???
    spi_dev_conf_hd.spics_io_num = CONFIG_FC_SPI_CS;
    spi_dev_conf_hd.flags = SPI_DEVICE_HALFDUPLEX; // Half Duplex mode, allow Rx length longer than Tx length
    spi_dev_conf_hd.queue_size = 7;

    // Full duplex device config
    memset(&spi_dev_conf_fd, 0, sizeof(spi_device_interface_config_t));
#ifndef CONFIG_FC_SPI_CLK_DEBUG
    spi_dev_conf_fd.clock_speed_hz = SPI_MASTER_FREQ_40M;
#else
    spi_dev_conf_fd.clock_speed_hz = SPI_MASTER_FREQ_8M;
#endif
    spi_dev_conf_fd.mode = 0; // CPOL = 0, CPHA = 0???
    spi_dev_conf_fd.spics_io_num = CONFIG_FC_SPI_CS;
    spi_dev_conf_fd.flags = 0; // Full duplex mode, Rx length must be <= Tx length
    spi_dev_conf_fd.queue_size = 7;

    ESP_LOGI(TAG, "Performing SPI init...");
    ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &spi_bus_config, 0));
    ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &spi_dev_conf_hd, &spi_dev_handle));
    ESP_LOGI(TAG, "SPI initialization finished, ready to rock!");
}

