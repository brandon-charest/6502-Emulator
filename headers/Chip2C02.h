#pragma once
#include <cstdint>
#include <memory>
#include "Cartridge.h"
// NES PPU Chip
class Chip2C02
{
public:
    Chip2C02();
    ~Chip2C02();


    // Communication to Main Bus
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

    // Communication with PPU Bus
    void ppuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool bReadOnly = false);

public: // Interface
    void ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void clock();
private:
    std::shared_ptr<Cartridge> cartridge;

    // Name table info, NES can work with up to 2 Name tables
    uint8_t tblName[2][1024];
    uint8_t tblPalette[32];
};