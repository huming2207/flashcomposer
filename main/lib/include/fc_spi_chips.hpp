#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <algorithm>
#include <map>

#include "fc_id_defs.hpp"

#define FC_SPI_FLASH_SECTOR_SIZE        4096
#define FC_SPI_FLASH_BLOCK_32_SIZE      32768
#define FC_SPI_FLASH_BLOCK_64_SIZE      65536

namespace fc
{
    struct spi_flash_mf_cmds
    {
        uint8_t sector_erase;
        uint8_t sector_erase_4ba;
        uint8_t block_erase_32;
        uint8_t block_erase_32_4ba;
        uint8_t block_erase_64;
        uint8_t block_erase_64_4ba;
        uint8_t chip_erase;
    };

    struct spi_flash_info
    {
        size_t full_size_kb = 0;
        uint32_t page_size_b = 256;
        std::string mf_name;
        std::string chip_name;
        spi_flash_mf_cmds mf_cmds;
    };

    struct spi_flash_ids
    {
        uint8_t mf_id = 0;
        uint16_t chip_id = 0;

        bool operator==(const spi_flash_ids& rhs) const
        {
            return rhs.chip_id == chip_id && rhs.mf_id == mf_id;
        };

        bool operator<(const spi_flash_ids& rhs) const
        {
            return mf_id < rhs.mf_id && chip_id < rhs.chip_id;
        }
    };

    // Per-factory based erase commands: { SE_3BA, SE_4BA, BE32_3BA, BE32_4BA, BE64_3BA, BE64_4BA, CE, RUID };
    // Leave commands to 0x00 if not supported
    const static spi_flash_mf_cmds winbond_erase_cmds        { 0x20, 0x20, 0x52, 0x52, 0xD8, 0xD8, 0xC7 };
    const static spi_flash_mf_cmds gigadevice_erase_cmds     { 0x20, 0x21, 0x52, 0x5C, 0xD8, 0xDC, 0x60 };
    const static spi_flash_mf_cmds macronix_erase_cmds       { 0x20, 0x21, 0x52, 0x5C, 0xD8, 0xDC, 0x60 };
    const static spi_flash_mf_cmds cypress_erase_cmds        { 0x20, 0x21, 0x52, 0x53, 0xD8, 0xDC, 0x60 };
    const static spi_flash_mf_cmds micron_erase_cmds         { 0x20, 0x21, 0x00, 0x00, 0xD8, 0xDC, 0xC7 };

    // Main definition of SPI Flash chips
    static const std::map<spi_flash_ids, spi_flash_info> fc_spi_chips = {
            {
                    { FC_MF_WINBOND_NEX, FC_CHIP_W25Q80 },
                    { 1024, 256, "Winbond", "W25Q80", winbond_erase_cmds }
            },
            {
                    { FC_MF_WINBOND_NEX, FC_CHIP_W25Q16 },
                    { 2048, 256, "Winbond", "W25Q16", winbond_erase_cmds }
            },
            {
                    { FC_MF_WINBOND_NEX, FC_CHIP_W25Q32 },
                    { 4096, 256, "Winbond", "W25Q32", winbond_erase_cmds }
            },
            {
                    { FC_MF_WINBOND_NEX, FC_CHIP_W25Q64 },
                    { 8192, 256, "Winbond", "W25Q64", winbond_erase_cmds }
            },
            {
                    { FC_MF_WINBOND_NEX, FC_CHIP_W25Q128 },
                    { 16384, 256, "Winbond", "W25Q128*V", winbond_erase_cmds }
            },
            {
                    { FC_MF_WINBOND_NEX, FC_CHIP_W25Q256 },
                    { 32768, 256, "Winbond", "W25Q256*V", winbond_erase_cmds }
            },
            {
                    { FC_MF_MACRONIX, FC_CHIP_MX25L1605 },
                    { 2048, 256, "Macronix", "MX25L16", macronix_erase_cmds }
            },
            {
                    { FC_MF_MACRONIX, FC_CHIP_MX25L3205 },
                    { 4096, 256, "Macronix", "MX25L32", macronix_erase_cmds }
            },
            {
                    { FC_MF_MACRONIX, FC_CHIP_MX25L6405 },
                    { 8192, 256, "Macronix", "MX25L64", macronix_erase_cmds }
            },
            {
                    { FC_MF_MACRONIX, FC_CHIP_MX25L12805D },
                    { 16384, 256, "Macronix", "MX25L128", macronix_erase_cmds }
            },
            {
                    { FC_MF_MACRONIX, FC_CHIP_MX25L256 },
                    { 32768, 256, "Macronix", "MX25L256", macronix_erase_cmds }
            },
            {
                    { FC_MF_GIGADEVICE, FC_CHIP_GD25Q32 },
                    { 4096, 256, "GigaDevice", "GD25Q32", gigadevice_erase_cmds }
            },
            {
                    { FC_MF_GIGADEVICE, FC_CHIP_GD25Q64 },
                    { 8192, 256, "GigaDevice", "GD25Q64", gigadevice_erase_cmds }
            },
            {
                    { FC_MF_GIGADEVICE, FC_CHIP_GD25Q128 },
                    { 16384, 256, "GigaDevice", "GD25Q128", gigadevice_erase_cmds }
            },
            {
                    { FC_MF_GIGADEVICE, FC_CHIP_GD25Q256 },
                    { 32768, 256, "GigaDevice", "GD25Q256", gigadevice_erase_cmds }
            }

    };
};
