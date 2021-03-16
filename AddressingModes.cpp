#include "headers/Chip6502.h"

// Addressing Modes
// Memory is between 0x0000 - 0xFFFF
// high byte is the 'page' and low byte is the offset into that page
uint8_t Chip6502::IMP()
{
    fetched = a; // Accumulator
    return 0;
}

uint8_t Chip6502::IMM()
{
    addr_abs = pc++;
    return 0;
}

uint8_t Chip6502::ZP0() 
{
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Chip6502::ZPX() 
{
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Chip6502::ZPY() 
{
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Chip6502::REL() 
{
    addr_rel = read(pc);
    pc++;

    if(addr_rel & 0x80)
    {
        addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t Chip6502::ABS() 
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t Chip6502::ABX() 
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += x;


    // if X register has changed to a different page
    if((addr_abs & 0xFF00) != (hi << 8))
    {
        return 1;
    }
    return 0;
}

uint8_t Chip6502::ABY() 
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;


    // if X register has changed to a different page
    if((addr_abs & 0xFF00) != (hi << 8))
    {
        return 1;
    }
    return 0;
}

// basically pointers
// There is also a known bug with this hardware where when incrementing the low byte you could
// overflow into the high byte thus changing the 'page'.
uint8_t Chip6502::IND() 
{
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;
    
    // The hardware bug was known and accounted for by NES game creaters and needs to be replicated.
    // The IND(xxFF) will fail and instead grab address 00xx instead of xx+1
    if(ptr_lo == 0x00FF)
    {
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else
    {
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }


    return 0;
}

uint8_t Chip6502::IZX() 
{
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00ff);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00ff);

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t Chip6502::IZY() 
{
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if((addr_abs & 0xFF00) != (hi << 8))
    {
        return 1;
    }
    return 0;
}
