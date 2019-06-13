#pragma once

#include <cstring>
#include <cstdint>

#include <driver/spi_master.h>
#include <esp_err.h>

namespace fc
{
    class fc_hal
    {
        public:
            static fc_hal& get_hal()
            {
                static fc_hal instance;
                return instance;
            };

            esp_err_t spi_write(const uint8_t *payload, size_t len);
            esp_err_t spi_write(uint8_t cmd);
            esp_err_t spi_read(const uint8_t *tx_payload, size_t tx_len, uint8_t *rx_payload, size_t rx_len);
            esp_err_t spi_read(uint8_t cmd, uint8_t *rx_payload, size_t rx_len);

        private:
            fc_hal();
            spi_device_handle_t device_handle = nullptr;

        public:
            fc_hal(fc_hal const &) = delete;
            void operator=(fc_hal const &) = delete;
    };
};
