#pragma once

#include <cstdint>

#include <esp_err.h>
#include "fc_hal.hpp"
#include "fc_spi_chips.hpp"

namespace fc
{
    typedef enum {
        READ_STATUS_REG_1   = 0x05U,
        READ_UNIQUE_ID      = 0x90U,
        READ_JEDEC_ID       = 0x92U,
        CHIP_ERASE          = 0xC7U,
    } spi_flash_cmd;

    class spi_flash
    {
        public:
            spi_flash();
            esp_err_t sector_erase(uint32_t start_addr, size_t len, uint32_t timeout_ms = 500);
            esp_err_t block_erase_32(uint32_t start_addr, size_t len, uint32_t timeout_ms = 5000);
            esp_err_t block_erase_64(uint32_t start_addr, size_t len, uint32_t timeout_ms = 5000);
            esp_err_t chip_erase(uint32_t timeout_ms = 15000);

            esp_err_t page_write(uint32_t addr, const uint8_t *payload, size_t len);
            esp_err_t byte_read(uint32_t addr, const uint32_t *payload, size_t len);

            bool get_busy();
            esp_err_t wait_when_busy(uint32_t timeout_ms = 10000);
            uint32_t get_total_size();
            uint16_t get_page_size();
            esp_err_t get_jedec_id(spi_flash_ids& ids);
            uint64_t get_unique_id();

            esp_err_t chip_wp_unlock();
            esp_err_t chip_pwr_off();
            esp_err_t chip_wp_relock();

            esp_err_t probe_flash();
            esp_err_t override_flash(spi_flash_ids &ids);

        private:
            fc_hal& hal;
            bool is_4ba = false;
            spi_flash_info chip_info{};
    };
};
