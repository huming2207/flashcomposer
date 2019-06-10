#pragma once

#include <cstdint>

#include <esp_err.h>

namespace fc
{
    class spi_flash
    {
        public:
            esp_err_t sector_erase(uint32_t addr);
            esp_err_t block_erase_32(uint32_t addr);
            esp_err_t block_erase_64(uint32_t addr);
            esp_err_t chip_erase();

            esp_err_t page_write(uint32_t addr, uint32_t payload[3]);
            esp_err_t byte_read(uint32_t addr, uint32_t *payload, size_t length);

            bool get_busy();
            uint32_t get_total_size();
            uint16_t get_page_size();
            uint8_t get_mf_id();
            uint8_t get_dev_id();
            uint64_t get_unique_id();

            esp_err_t chip_wp_unlock();
            esp_err_t chip_pwr_off();
            esp_err_t chip_wp_relock();
    };
};
