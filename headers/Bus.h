
#pragma once
#include <array>
#include <cstdint>
#include "Chip6502.h"

const uint8_t MAX_MEM = 1024 * 64;

class Bus 
{
public:
    Bus();
    ~Bus();

    // Devices connected to bus
    Chip6502 cpu;
    std::array<uint8_t, MAX_MEM> ram;

    // Bus Read/Write
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);

private:
    // Clean Ram
    void clear();
};