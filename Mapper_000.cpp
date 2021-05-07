#include "headers/Mapper_000.h"
// Mapper 000 (NROM)
// https://wiki.nesdev.com/w/index.php/NROM

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks)
    : Mapper(prgBanks, chrBanks)
{
}

Mapper_000::~Mapper_000()
{
}


bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        // We need to know how many 16k PRG pages were loaded.
        // if there is more then 1 then we need to account for the range
        // to be 32k and offset from 0
        // if PRGROM is 16KB
        //     CPU Address Bus          PRG ROM
        //     0x8000 -> 0xBFFF: Map    0x0000 -> 0x3FFF
        //     0xC000 -> 0xFFFF: Mirror 0x0000 -> 0x3FFF
        // if PRGROM is 32KB
        //     CPU Address Bus          PRG ROM
        //     0x8000 -> 0xFFFF: Map    0x0000 -> 0x7FFF
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    // No mapping required for PPU
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) 
{
    return false;
}