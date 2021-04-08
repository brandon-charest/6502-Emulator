#include <stdexcept>
#include <string>
#include "headers/Chip2C02.h"

Chip2C02::Chip2C02(/* args */)
{
}

Chip2C02::~Chip2C02()
{
}

void Chip2C02::cpuWrite(uint16_t addr, uint8_t data) 
{
    switch (addr)
    {
    case 0x0000: // Control
        break;
    case 0x0001: // Mask
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        break;
    case 0x0007: // PPU Data
        break;
    default:
        throw std::invalid_argument(std::string("Invalid CPU Write Address: ") + std::to_string(addr));
    }
}

uint8_t Chip2C02::cpuRead(uint16_t addr, bool bReadOnly) 
{
    uint8_t data = 0x00;

    switch (addr)
    {
    case 0x0000: // Control
        break;
    case 0x0001: // Mask
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        break;
    case 0x0007: // PPU Data
        break;
    default: // Should never get here
        throw std::invalid_argument(std::string("Invalid CPU Read Address: ") + std::to_string(addr));
    }

    return data;
}

void Chip2C02::ppuWrite(uint16_t addr, uint8_t data) 
{
    // address gets masked to make sure
    // data does not get written beyond its addressable range
    addr &= 0x3FFF;
}

uint8_t Chip2C02::ppuRead(uint16_t addr, bool bReadOnly) 
{
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    return data;
}

void Chip2C02::ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge) 
{
    this->cartridge = cartridge;
}

void Chip2C02::clock() 
{
    
}