#pragma once

#include <esp_err.h>

namespace fc
{
    class fc_hal
    {
        public:
            static fc_hal& get_hal();
            esp_err_t spi_write(const uint8_t *payload, size_t len);
            esp_err_t spi_write(uint8_t cmd, uint32_t addr);
            esp_err_t spi_read(uint8_t *payload, size_t len);

        private:
            fc_hal();

        public:
            fc_hal(fc_hal const &) = delete;
            void operator=(fc_hal const &) = delete;
    };
};
