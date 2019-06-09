#include "fc_hal.hpp"

using namespace fc;

esp_err_t fc_hal::spi_write(const uint8_t *payload, size_t len)
{
    return 0;
}

esp_err_t fc_hal::spi_write(const uint8_t cmd, const uint32_t addr)
{
    return 0;
}

esp_err_t fc_hal::spi_read(uint8_t *payload, size_t len)
{
    return 0;
}

fc_hal &fc_hal::get_hal()
{
    static fc_hal instance;
    return instance;
}

fc_hal::fc_hal()
{

}
