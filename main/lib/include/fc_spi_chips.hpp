#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <map>

#include "fc_id_defs.hpp"

namespace fc
{
    struct spi_flash_info
    {
        size_t full_size_kb = 0;
        uint32_t page_size_b = 256;
        const std::string mf_name;
        const std::string chip_name;
    };

    struct spi_flash_ids
    {
        uint8_t mf_id = 0;
        uint16_t chip_id = 0;

        bool operator==(spi_flash_ids& rhs) const
        {
            return rhs.chip_id == chip_id && rhs.mf_id == mf_id;
        };

        bool operator<(spi_flash_ids& rhs) const
        {
            return mf_id < rhs.mf_id && chip_id < rhs.chip_id;
        }
    };

    static const std::map<spi_flash_ids, spi_flash_info> fc_spi_chips = {
            {
                { FC_MF_WINBOND_NEX, FC_CHIP_W25Q128 },
                { 16384, 256, "Winbond", "W25Q128*V" }
            },
            {
                { FC_MF_WINBOND_NEX, FC_CHIP_W25Q256 },
                { 32768, 256, "Winbond", "W25Q256*V" }
            }
    };
};
