#include "fc_spi_flash.hpp"

using namespace fc;

esp_err_t spi_flash::sector_erase(uint32_t addr)
{
    return 0;
}

esp_err_t spi_flash::chip_erase()
{
    return 0;
}

esp_err_t spi_flash::block_erase_32(uint32_t addr)
{
    return 0;
}

esp_err_t spi_flash::block_erase_64(uint32_t addr)
{
    return 0;
}

esp_err_t spi_flash::page_write(uint32_t addr, uint32_t *payload)
{
    return 0;
}

esp_err_t spi_flash::byte_read(uint32_t addr, uint32_t *payload, size_t length)
{
    return 0;
}

bool spi_flash::get_busy()
{
    return false;
}

uint32_t spi_flash::get_total_size()
{
    return 0;
}

uint16_t spi_flash::get_page_size()
{
    return 0;
}

uint8_t spi_flash::get_mf_id()
{
    return 0;
}

uint8_t spi_flash::get_dev_id()
{
    return 0;
}

uint64_t spi_flash::get_unique_id()
{
    return 0;
}

esp_err_t spi_flash::chip_wp_unlock()
{
    return 0;
}

esp_err_t spi_flash::chip_pwr_off()
{
    return 0;
}

esp_err_t spi_flash::chip_wp_relock()
{
    return 0;
}



