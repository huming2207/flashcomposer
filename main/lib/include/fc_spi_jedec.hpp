#pragma once

#include "fc_spi_flash.hpp"

namespace fc
{
    class spi_jedec : public spi_flash
    {        
        public:
            esp_err_t sector_erase(uint32_t addr) override;
            esp_err_t block_erase(uint32_t addr) override;
            esp_err_t chip_erase() override;
    
            esp_err_t page_write(uint32_t addr, uint32_t payload[3]) override;
            esp_err_t byte_read(uint32_t addr, const uint32_t *payload, const size_t *length) override;
    
            bool get_busy() override;
            uint32_t get_total_size() override;
            uint16_t get_page_size() override;
            uint8_t get_mf_id() override;
            uint8_t get_dev_id() override;
            uint64_t get_unique_id() override;
    
            esp_err_t chip_wp_unlock() override;
            esp_err_t chip_pwr_off() override;
            esp_err_t chip_wp_relock() override;

    };
};
