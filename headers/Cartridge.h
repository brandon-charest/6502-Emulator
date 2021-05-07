#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include "Mapper_000.h"

class Cartridge
{
public:
    Cartridge(const std::string &sFileName);
    ~Cartridge();

     // Communication to Main Bus
    bool cpuWrite(uint16_t addr, uint8_t data);
    bool cpuRead(uint16_t addr, uint8_t &data);

    // Communication with PPU Bus
    bool ppuWrite(uint16_t addr, uint8_t data);
    bool ppuRead(uint16_t addr, uint8_t &data);

private:

    void openFile(const std::string &sFileName);
    const uint16_t PRG_BANK_SIZE = 16384;
    const uint16_t CHR_BANK_SIZE = 8192;
    // Memory for program banks
    std::vector<uint8_t> vPRGMemory;
    // Memory for character/sprites
    std::vector<uint8_t> vCHRMemory;

    // Mapper are we using
    uint8_t nMapperID = 0;
    // How many PRG Banks
    uint8_t nPRGBanks = 0;
    // How mant CHR Banks
    uint8_t nCHRBanks = 0;

    std::shared_ptr<Mapper> pMapper;

};

