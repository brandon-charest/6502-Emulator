
#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "Chip6502.h"
#include "Chip2C02.h"
#include "Cartridge.h"

const uint32_t MAX_MEM = 2048;

class Bus 
{
public:
    Bus();
    ~Bus();

    // Devices connected to bus
    Chip6502 cpu;
    // Picture Processing Unit
    Chip2C02 ppu;
    // Cartridge
    std::shared_ptr<Cartridge> cartridge;
    // RAM
    std::array<uint8_t, MAX_MEM> cpuRam;

    // Bus Read/Write
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public: // System Interface
    void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void reset();
    void clock();

private:

    // Clean Ram
    void clear();

    // Counter for how many clock() is called
    uint32_t nSystemClockCounter = 0;
};