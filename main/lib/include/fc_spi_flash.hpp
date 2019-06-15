#pragma once

#include <cstdint>

#include <esp_err.h>
#include "fc_hal.hpp"
#include "fc_spi_chips.hpp"

namespace fc
{
    namespace cmd_def
    {
        typedef enum {
            WRITE_STATUS_REG_1  = 0x01U,
            PAGE_PROGRAM        = 0x02U,
            READ_DATA           = 0x03U,
            WRITE_DISABLE       = 0x04U,
            READ_STATUS_REG_1   = 0x05U,
            WRITE_ENABLE        = 0x06U,
            FAST_READ_DATA      = 0x0BU,
            BLOCK_ERASE_32      = 0x52U,
            BLOCK_ERASE_32_GD   = 0x5CU, // For
            READ_UNIQUE_ID      = 0x4BU,
            READ_MF_DEV_ID      = 0x90U,
            READ_JEDEC_ID       = 0x92U,
            RELEASE_POWER_DOWN  = 0xABU,
            ENTER_4BA           = 0xB7U,
            POWER_DOWN          = 0xB9U,
            EXIT_4BA            = 0xE9U
        } spi_flash_cmd;
    }

    class spi_flash
    {
        public:
            spi_flash();
            esp_err_t sector_erase(uint32_t start_addr, size_t len, uint32_t timeout_ms = 500);
            esp_err_t block_erase_32(uint32_t start_addr, size_t len, uint32_t timeout_ms = 5000);
            esp_err_t block_erase_64(uint32_t start_addr, size_t len, uint32_t timeout_ms = 5000);
            esp_err_t chip_erase(uint32_t timeout_ms = 15000);

            esp_err_t page_program(uint32_t addr, const uint8_t *payload, size_t len);
            esp_err_t byte_read(uint32_t addr, uint8_t *rx_payload, size_t len, bool fast_read);

            bool get_busy();
            esp_err_t wait_when_busy(uint32_t timeout_ms = 10000);
            uint32_t get_total_size();
            uint16_t get_page_size();
            esp_err_t get_jedec_id(spi_flash_ids& ids);
            uint64_t get_unique_id();

            esp_err_t enable_write();
            esp_err_t chip_pwr_off();
            esp_err_t disable_write();

            esp_err_t probe_flash();
            esp_err_t override_flash(spi_flash_ids &ids);

        private:
            fc_hal& hal;
            bool is_4ba = false;
            spi_flash_info chip_info{};
            esp_err_t enter_4ba_mode();
            esp_err_t exit_4ba_mode();
            esp_err_t common_erase(uint8_t cmd, uint32_t unit, uint32_t start_addr, size_t len, uint32_t timeout_ms);
    };
};
