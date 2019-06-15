#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "fc_spi_flash.hpp"

#define TAG "spi_flash"

using namespace fc;

spi_flash::spi_flash() : hal(fc_hal::get_hal())
{
    probe_flash();
}

esp_err_t spi_flash::sector_erase(uint32_t start_addr, size_t len, uint32_t timeout_ms)
{
    auto cmd = is_4ba ? chip_info.erase_cmds.sector_erase_4ba : chip_info.erase_cmds.sector_erase;
    return common_erase(cmd, FC_SPI_FLASH_SECTOR_SIZE, start_addr, len, timeout_ms);
}

esp_err_t spi_flash::chip_erase(uint32_t timeout_ms)
{
    if(chip_info.erase_cmds.chip_erase == 0) return ESP_ERR_NOT_SUPPORTED;
    esp_err_t ret = hal.spi_write(chip_info.erase_cmds.chip_erase);
    ret = ret ?: wait_when_busy(timeout_ms);
    return ret;
}

esp_err_t spi_flash::block_erase_32(uint32_t start_addr, size_t len, uint32_t timeout_ms)
{
    auto cmd = is_4ba ? chip_info.erase_cmds.block_erase_32_4ba : chip_info.erase_cmds.block_erase_32;
    return common_erase(cmd, FC_SPI_FLASH_BLOCK_32_SIZE, start_addr, len, timeout_ms);
}

esp_err_t spi_flash::block_erase_64(uint32_t start_addr, size_t len, uint32_t timeout_ms)
{
    auto cmd = is_4ba ? chip_info.erase_cmds.block_erase_64_4ba : chip_info.erase_cmds.block_erase_64;
    return common_erase(cmd, FC_SPI_FLASH_BLOCK_64_SIZE, start_addr, len, timeout_ms);
}

esp_err_t spi_flash::page_program(uint32_t addr, const uint8_t *payload, size_t len)
{
    return ESP_OK;
}

esp_err_t spi_flash::byte_read(uint32_t addr, uint8_t *rx_payload, size_t len, bool fast_read = true)
{
    // FAST_READ needs an extra dummy byte
    auto cmd = fast_read ? cmd_def::FAST_READ_DATA : cmd_def::READ_DATA;
    const uint8_t dummy = 0;
    return hal.spi_read(cmd, addr, &dummy, fast_read ? 1 : 0, rx_payload, len, is_4ba);
}

bool spi_flash::get_busy()
{
    uint8_t result = 0;
    esp_err_t ret = hal.spi_read(cmd_def::READ_STATUS_REG_1, &result, 1);
    if(ret != ESP_OK) return true;

    return (result & 0x01u) == 1; // Probe Bit 0
}

esp_err_t spi_flash::wait_when_busy(uint32_t timeout_ms)
{
    bool is_busy = true;
    while(is_busy && timeout_ms > 1) {
        is_busy = get_busy();
        timeout_ms--;
        vTaskDelay(pdMS_TO_TICKS(3));
    }

    if(timeout_ms < 1) return ESP_ERR_TIMEOUT;
    else return ESP_OK;
}

uint32_t spi_flash::get_total_size()
{
    return ESP_OK;
}

uint16_t spi_flash::get_page_size()
{
    return ESP_OK;
}

esp_err_t spi_flash::get_jedec_id(spi_flash_ids& ids)
{
    uint8_t result[3] = { 0 };
    esp_err_t ret = hal.spi_read(cmd_def::READ_JEDEC_ID, result, 3);
    if(ret != ESP_OK) return ret;

    ids.mf_id = result[0];
    ids.chip_id = (uint16_t)((uint8_t)(result[1] << 8u) | result[2]); // Cast it to shut up Clang-tidy warning

    return ESP_OK;
}

uint64_t spi_flash::get_unique_id()
{
    return ESP_OK;
}

esp_err_t spi_flash::enable_write()
{
    return hal.spi_write(cmd_def::WRITE_ENABLE);
}

esp_err_t spi_flash::chip_pwr_off()
{
    return hal.spi_write(cmd_def::POWER_DOWN);
}

esp_err_t spi_flash::disable_write()
{
    return hal.spi_write(cmd_def::WRITE_DISABLE);
}

esp_err_t spi_flash::probe_flash()
{
    spi_flash_ids ids{};
    esp_err_t ret = get_jedec_id(ids);
    if(ret != ESP_OK) return ret;

    chip_info = fc_spi_chips.at(ids);

    return ESP_OK;
}

esp_err_t spi_flash::override_flash(spi_flash_ids &ids)
{
    chip_info = fc_spi_chips.at(ids);
    if(chip_info.full_size_kb == 0) return ESP_ERR_NOT_FOUND; // If not found, operator[] will make a dummy object

    return ESP_OK;
}

esp_err_t spi_flash::enter_4ba_mode()
{
    return hal.spi_write(cmd_def::ENTER_4BA);
}

esp_err_t spi_flash::exit_4ba_mode()
{
    return hal.spi_write(cmd_def::EXIT_4BA);
}

esp_err_t spi_flash::common_erase(const uint8_t cmd, uint32_t unit, uint32_t start_addr, size_t len, uint32_t timeout_ms)
{
    if(cmd == 0) return ESP_ERR_NOT_SUPPORTED;

    esp_err_t ret = ESP_OK;
    for(size_t idx = start_addr; idx < start_addr + len; idx += unit) {
        ret = ret ?: hal.spi_write(cmd, idx, nullptr, 0, is_4ba);
        ret = ret ?: wait_when_busy(timeout_ms);
    }

    return ret;
}
