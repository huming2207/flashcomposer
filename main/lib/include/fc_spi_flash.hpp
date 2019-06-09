#pragma once

#include <cstdint>

#include <esp_err.h>

namespace fc
{
    class spi_flash
    {
        public:
            virtual esp_err_t sector_erase(uint32_t addr) = 0;
            virtual esp_err_t block_erase(uint32_t addr) = 0;
            virtual esp_err_t chip_erase() = 0;

            virtual esp_err_t page_write(uint32_t addr, uint32_t payload[3]) = 0;
            virtual esp_err_t byte_read(uint32_t addr, const uint32_t *payload, const size_t *length) = 0;

            virtual bool get_busy() = 0;
            virtual uint32_t get_total_size() = 0;
            virtual uint16_t get_page_size() = 0;
            virtual uint8_t get_mf_id() = 0;
            virtual uint8_t get_dev_id() = 0;
            virtual uint64_t get_unique_id() = 0;

            virtual esp_err_t chip_wp_unlock() = 0;
            virtual esp_err_t chip_pwr_off() = 0;
            virtual esp_err_t chip_wp_relock() = 0;
    };
};
