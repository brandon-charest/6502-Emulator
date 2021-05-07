#include "headers/Cartridge.h"


Cartridge::Cartridge(const std::string &sFileName)
{
    // https://wiki.nesdev.com/w/index.php/INES
    /*
    0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
    4: Size of PRG ROM in 16 KB units
    5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
    6: Flags 6 - Mapper, mirroring, battery, trainer
    7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
    8: Flags 8 - PRG-RAM size (rarely used extension)
    9: Flags 9 - TV system (rarely used extension)
    10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
    11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
    */
    struct sHeader
    {
        char name[4];
        uint8_t pgr_rom_pages;
        uint8_t chr_rom_pages;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if(ifs.is_open())
    {
        // Read file header
        ifs.read((char*)&header, sizeof(header));

        // Skip "trainer" data which is 512 bytes of code usually loaded into $7000
        // before the game data.
        if(header.mapper1 & 0x04)
        {
            ifs.seekg(512, std::ios_base::cur);
        }

        // Determine Mapper ID
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

        uint8_t nFileType = 1;

        // There are three iNES file types, NES 2.0, iNES, archaic INES
        // only going to care about type 1 for now
        // if(nFileType == 0)
        // {
        // }
        if(nFileType == 1)
        {
            nPRGBanks = header.pgr_rom_pages;
            vPRGMemory.resize(nPRGBanks * PRG_BANK_SIZE);
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = header.chr_rom_pages;
            vCHRMemory.resize(nCHRBanks * CHR_BANK_SIZE);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }
        // if(nFileType == 2)
        // {
        // }


        switch (nMapperID)
        {
        case 0:
            pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
            break;
        
        default:
            break;
        }

        ifs.close();

    }
}

Cartridge::~Cartridge()
{
}

void Cartridge::openFile(const std::string &sFileName)
{
   
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) 
{
    uint32_t mapped_addr = 0;
    if(pMapper->cpuMapWrite(addr, mapped_addr))
    {
        data = vPRGMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) 
{
    uint32_t mapped_addr = 0;
    if(pMapper->cpuMapRead(addr, mapped_addr))
    {
        data = vPRGMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) 
{
    uint32_t mapped_addr = 0;
    if(pMapper->ppuMapWrite(addr, mapped_addr))
    {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t &data) 
{
    uint32_t mapped_addr = 0;
    if(pMapper->ppuMapRead(addr, mapped_addr))
    {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    return false;
}

