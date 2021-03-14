#include "headers/Chip6502.h"
#include "headers/Bus.h"

Chip6502::Chip6502() {}

Chip6502::~Chip6502() {}

uint8_t Chip6502::read(uint16_t addr) 
{
    return bus->read(addr, false);
}

void Chip6502::write(uint16_t addr, uint8_t data) 
{
    bus->write(addr, data);
}
