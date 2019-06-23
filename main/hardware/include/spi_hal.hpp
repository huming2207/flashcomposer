#pragma once

#include <cstring>
#include <cstdint>

#include <driver/spi_master.h>
#include <esp_err.h>

namespace fc::hardware
{
    class spi_hal
    {
        public:
            static spi_hal& get_hal()
            {
                static spi_hal instance;
                return instance;
            };

            esp_err_t spi_write(const uint8_t *payload, size_t len);
            esp_err_t spi_write(uint8_t cmd);
            esp_err_t spi_write(uint8_t cmd, uint32_t addr, const uint8_t *tx_payload, size_t tx_len, bool is_4ba);
            esp_err_t spi_read(const uint8_t *tx_payload, size_t tx_len, uint8_t *rx_payload, size_t rx_len);
            esp_err_t spi_read(uint8_t cmd, uint8_t *rx_payload, size_t rx_len);
            esp_err_t spi_read(uint8_t cmd, uint32_t addr, const uint8_t *tx_payload, size_t tx_len,
                               uint8_t *rx_payload, size_t rx_len, bool is_4ba);

            esp_err_t spi_set_full_duplex();
            esp_err_t spi_set_half_duplex();

        private:
            spi_hal();
            spi_bus_config_t spi_bus_config{};
            spi_device_handle_t spi_dev_handle = nullptr;
            spi_device_interface_config_t spi_dev_conf_hd{};
            spi_device_interface_config_t spi_dev_conf_fd{};

        public:
            spi_hal(spi_hal const &) = delete;
            void operator=(spi_hal const &) = delete;
    };
};
