#pragma once
#include <cstdint>

class Cartridge
{
public:
    Cartridge();
    ~Cartridge();

     // Communication to Main Bus
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

    // Communication with PPU Bus
    void ppuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool bReadOnly = false);
};

