#include "fc_spi_jedec.hpp"

using namespace fc;

esp_err_t spi_jedec::sector_erase(uint32_t addr)
{
    return 0;
}

esp_err_t spi_jedec::chip_erase()
{
    return 0;
}

esp_err_t spi_jedec::block_erase(uint32_t addr)
{
    return 0;
}

esp_err_t spi_jedec::page_write(uint32_t addr, uint32_t *payload)
{
    return 0;
}

esp_err_t spi_jedec::byte_read(uint32_t addr, const uint32_t *payload, const size_t *length)
{
    return 0;
}

bool spi_jedec::get_busy()
{
    return false;
}

uint32_t spi_jedec::get_total_size()
{
    return 0;
}

uint16_t spi_jedec::get_page_size()
{
    return 0;
}

uint8_t spi_jedec::get_mf_id()
{
    return 0;
}

uint8_t spi_jedec::get_dev_id()
{
    return 0;
}

uint64_t spi_jedec::get_unique_id()
{
    return 0;
}

esp_err_t spi_jedec::chip_wp_unlock()
{
    return 0;
}

esp_err_t spi_jedec::chip_pwr_off()
{
    return 0;
}

esp_err_t spi_jedec::chip_wp_relock()
{
    return 0;
}


