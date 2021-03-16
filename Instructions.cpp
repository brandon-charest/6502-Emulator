#include "headers/Chip6502.h"

uint8_t Chip6502::ADC() 
{
     fetch();
     uint16_t val = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
     setFlag(C, val > 255);
     setFlag(Z, (val & 0x00FF) == 0);
     setFlag(N, (val & 0x80));
     // V = ~(a ^ m) & (a ^ r)
     //setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)val)) & 0x0080);
}

uint8_t Chip6502::AND() 
{
    fetch();
    a = a & fetched;
    // if all bits 0
    setFlag(Z, a == 0x00);
    // if bit 7 == 1
    setFlag(N, a & 0x80);
    return 1;
}

uint8_t Chip6502::ASL() 
{
    
}

uint8_t Chip6502::BCC() 
{
    if(GetFlag(C) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::BCS() 
{
    if(GetFlag(C) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::BEQ() 
{
    if(GetFlag(Z) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::BIT() 
{
    
}

uint8_t Chip6502::BMI() 
{
    if(GetFlag(N) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::BNE() 
{
    if(GetFlag(Z) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::BPL() 
{ 
    if(GetFlag(N) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
    
}

uint8_t Chip6502::BRK() 
{
    
}

uint8_t Chip6502::BVC() 
{
    
    if(GetFlag(V) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::BVS() 
{
    
    if(GetFlag(V) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

uint8_t Chip6502::CLC() 
{
    setFlag(C, false);
    return 0;
}

uint8_t Chip6502::CLD() 
{
    setFlag(D, false);
    return 0;
}

uint8_t Chip6502::CLI() 
{
    setFlag(I, false);
    return 0;
}

uint8_t Chip6502::CLV() 
{
    setFlag(V, false);
    return 0;
}

uint8_t Chip6502::CMP() 
{
    
}

uint8_t Chip6502::CPX() 
{
    
}

uint8_t Chip6502::CPY() 
{
    
}

uint8_t Chip6502::DEC() 
{
    
}

uint8_t Chip6502::DEX() 
{
    
}

uint8_t Chip6502::DEY() 
{
    
}

uint8_t Chip6502::EOR() 
{
    
}

uint8_t Chip6502::INC() 
{
    
}

uint8_t Chip6502::INX() 
{
    
}

uint8_t Chip6502::INY() 
{
    
}

uint8_t Chip6502::JMP() 
{
    
}

uint8_t Chip6502::JSR() 
{
    
}

uint8_t Chip6502::LDA() 
{
    
}

uint8_t Chip6502::LDX() 
{
    
}

uint8_t Chip6502::LDY() 
{
    
}

uint8_t Chip6502::LSR() 
{
    
}

uint8_t Chip6502::NOP() 
{
    
}

uint8_t Chip6502::ORA() 
{
    
}

uint8_t Chip6502::PHA() 
{
    
}

uint8_t Chip6502::PHP() 
{
    
}

uint8_t Chip6502::PLA() 
{
    
}

uint8_t Chip6502::PLP() 
{
    
}

uint8_t Chip6502::ROL() 
{
    
}

uint8_t Chip6502::ROR() 
{
    
}

uint8_t Chip6502::RTI() 
{
    
}

uint8_t Chip6502::RTS() 
{
    
}

uint8_t Chip6502::SBC() 
{
    
}

uint8_t Chip6502::SEC() 
{
    setFlag(C, true);
    return 0;
}

uint8_t Chip6502::SED() 
{
    setFlag(D, false);
    return 0;
}

uint8_t Chip6502::SEI() 
{
    setFlag(I, false);
    return 0;
}

uint8_t Chip6502::STA() 
{
    
}

uint8_t Chip6502::STX() 
{
    
}

uint8_t Chip6502::STY() 
{
    
}

uint8_t Chip6502::TAX() 
{
    
}

uint8_t Chip6502::TAY() 
{
    
}

uint8_t Chip6502::TSX() 
{
    
}

uint8_t Chip6502::TXA() 
{
    
}

uint8_t Chip6502::TXS() 
{
    
}

uint8_t Chip6502::TYA() 
{
    
}

uint8_t Chip6502::XXX() 
{
    
}