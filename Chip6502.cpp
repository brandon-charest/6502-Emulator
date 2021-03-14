#include "headers/Chip6502.h"
#include "headers/Bus.h"


Chip6502::Chip6502() 
{
   
    // I did not make this look up. Only modified it slightly to keep in line with my own implementation
    // Thanks to OneLoneCoder for spending literally days making this.

    // This Op Code Matrix is made to mirror the original table found below on pg 10.
    // http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf
    using M = Chip6502;
    const std::vector<INSTRUCTION> lookup = 
    {
        { "BRK", &M::BRK, &M::IMM, 7 },{ "ORA", &M::ORA, &M::IZX, 6 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 3 },{ "ORA", &M::ORA, &M::ZP0, 3 },{ "ASL", &M::ASL, &M::ZP0, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "PHP", &M::PHP, &M::IMP, 3 },{ "ORA", &M::ORA, &M::IMM, 2 },{ "ASL", &M::ASL, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::NOP, &M::IMP, 4 },{ "ORA", &M::ORA, &M::ABS, 4 },{ "ASL", &M::ASL, &M::ABS, 6 },{ "???", &M::XXX, &M::IMP, 6 },
        { "BPL", &M::BPL, &M::REL, 2 },{ "ORA", &M::ORA, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 4 },{ "ORA", &M::ORA, &M::ZPX, 4 },{ "ASL", &M::ASL, &M::ZPX, 6 },{ "???", &M::XXX, &M::IMP, 6 },{ "CLC", &M::CLC, &M::IMP, 2 },{ "ORA", &M::ORA, &M::ABY, 4 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 7 },{ "???", &M::NOP, &M::IMP, 4 },{ "ORA", &M::ORA, &M::ABX, 4 },{ "ASL", &M::ASL, &M::ABX, 7 },{ "???", &M::XXX, &M::IMP, 7 },
        { "JSR", &M::JSR, &M::ABS, 6 },{ "AND", &M::AND, &M::IZX, 6 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "BIT", &M::BIT, &M::ZP0, 3 },{ "AND", &M::AND, &M::ZP0, 3 },{ "ROL", &M::ROL, &M::ZP0, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "PLP", &M::PLP, &M::IMP, 4 },{ "AND", &M::AND, &M::IMM, 2 },{ "ROL", &M::ROL, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "BIT", &M::BIT, &M::ABS, 4 },{ "AND", &M::AND, &M::ABS, 4 },{ "ROL", &M::ROL, &M::ABS, 6 },{ "???", &M::XXX, &M::IMP, 6 },
        { "BMI", &M::BMI, &M::REL, 2 },{ "AND", &M::AND, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 4 },{ "AND", &M::AND, &M::ZPX, 4 },{ "ROL", &M::ROL, &M::ZPX, 6 },{ "???", &M::XXX, &M::IMP, 6 },{ "SEC", &M::SEC, &M::IMP, 2 },{ "AND", &M::AND, &M::ABY, 4 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 7 },{ "???", &M::NOP, &M::IMP, 4 },{ "AND", &M::AND, &M::ABX, 4 },{ "ROL", &M::ROL, &M::ABX, 7 },{ "???", &M::XXX, &M::IMP, 7 },
        { "RTI", &M::RTI, &M::IMP, 6 },{ "EOR", &M::EOR, &M::IZX, 6 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 3 },{ "EOR", &M::EOR, &M::ZP0, 3 },{ "LSR", &M::LSR, &M::ZP0, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "PHA", &M::PHA, &M::IMP, 3 },{ "EOR", &M::EOR, &M::IMM, 2 },{ "LSR", &M::LSR, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "JMP", &M::JMP, &M::ABS, 3 },{ "EOR", &M::EOR, &M::ABS, 4 },{ "LSR", &M::LSR, &M::ABS, 6 },{ "???", &M::XXX, &M::IMP, 6 },
        { "BVC", &M::BVC, &M::REL, 2 },{ "EOR", &M::EOR, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 4 },{ "EOR", &M::EOR, &M::ZPX, 4 },{ "LSR", &M::LSR, &M::ZPX, 6 },{ "???", &M::XXX, &M::IMP, 6 },{ "CLI", &M::CLI, &M::IMP, 2 },{ "EOR", &M::EOR, &M::ABY, 4 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 7 },{ "???", &M::NOP, &M::IMP, 4 },{ "EOR", &M::EOR, &M::ABX, 4 },{ "LSR", &M::LSR, &M::ABX, 7 },{ "???", &M::XXX, &M::IMP, 7 },
        { "RTS", &M::RTS, &M::IMP, 6 },{ "ADC", &M::ADC, &M::IZX, 6 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 3 },{ "ADC", &M::ADC, &M::ZP0, 3 },{ "ROR", &M::ROR, &M::ZP0, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "PLA", &M::PLA, &M::IMP, 4 },{ "ADC", &M::ADC, &M::IMM, 2 },{ "ROR", &M::ROR, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "JMP", &M::JMP, &M::IND, 5 },{ "ADC", &M::ADC, &M::ABS, 4 },{ "ROR", &M::ROR, &M::ABS, 6 },{ "???", &M::XXX, &M::IMP, 6 },
        { "BVS", &M::BVS, &M::REL, 2 },{ "ADC", &M::ADC, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 4 },{ "ADC", &M::ADC, &M::ZPX, 4 },{ "ROR", &M::ROR, &M::ZPX, 6 },{ "???", &M::XXX, &M::IMP, 6 },{ "SEI", &M::SEI, &M::IMP, 2 },{ "ADC", &M::ADC, &M::ABY, 4 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 7 },{ "???", &M::NOP, &M::IMP, 4 },{ "ADC", &M::ADC, &M::ABX, 4 },{ "ROR", &M::ROR, &M::ABX, 7 },{ "???", &M::XXX, &M::IMP, 7 },
        { "???", &M::NOP, &M::IMP, 2 },{ "STA", &M::STA, &M::IZX, 6 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 6 },{ "STY", &M::STY, &M::ZP0, 3 },{ "STA", &M::STA, &M::ZP0, 3 },{ "STX", &M::STX, &M::ZP0, 3 },{ "???", &M::XXX, &M::IMP, 3 },{ "DEY", &M::DEY, &M::IMP, 2 },{ "???", &M::NOP, &M::IMP, 2 },{ "TXA", &M::TXA, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "STY", &M::STY, &M::ABS, 4 },{ "STA", &M::STA, &M::ABS, 4 },{ "STX", &M::STX, &M::ABS, 4 },{ "???", &M::XXX, &M::IMP, 4 },
        { "BCC", &M::BCC, &M::REL, 2 },{ "STA", &M::STA, &M::IZY, 6 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 6 },{ "STY", &M::STY, &M::ZPX, 4 },{ "STA", &M::STA, &M::ZPX, 4 },{ "STX", &M::STX, &M::ZPY, 4 },{ "???", &M::XXX, &M::IMP, 4 },{ "TYA", &M::TYA, &M::IMP, 2 },{ "STA", &M::STA, &M::ABY, 5 },{ "TXS", &M::TXS, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 5 },{ "???", &M::NOP, &M::IMP, 5 },{ "STA", &M::STA, &M::ABX, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "???", &M::XXX, &M::IMP, 5 },
        { "LDY", &M::LDY, &M::IMM, 2 },{ "LDA", &M::LDA, &M::IZX, 6 },{ "LDX", &M::LDX, &M::IMM, 2 },{ "???", &M::XXX, &M::IMP, 6 },{ "LDY", &M::LDY, &M::ZP0, 3 },{ "LDA", &M::LDA, &M::ZP0, 3 },{ "LDX", &M::LDX, &M::ZP0, 3 },{ "???", &M::XXX, &M::IMP, 3 },{ "TAY", &M::TAY, &M::IMP, 2 },{ "LDA", &M::LDA, &M::IMM, 2 },{ "TAX", &M::TAX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "LDY", &M::LDY, &M::ABS, 4 },{ "LDA", &M::LDA, &M::ABS, 4 },{ "LDX", &M::LDX, &M::ABS, 4 },{ "???", &M::XXX, &M::IMP, 4 },
        { "BCS", &M::BCS, &M::REL, 2 },{ "LDA", &M::LDA, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 5 },{ "LDY", &M::LDY, &M::ZPX, 4 },{ "LDA", &M::LDA, &M::ZPX, 4 },{ "LDX", &M::LDX, &M::ZPY, 4 },{ "???", &M::XXX, &M::IMP, 4 },{ "CLV", &M::CLV, &M::IMP, 2 },{ "LDA", &M::LDA, &M::ABY, 4 },{ "TSX", &M::TSX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 4 },{ "LDY", &M::LDY, &M::ABX, 4 },{ "LDA", &M::LDA, &M::ABX, 4 },{ "LDX", &M::LDX, &M::ABY, 4 },{ "???", &M::XXX, &M::IMP, 4 },
        { "CPY", &M::CPY, &M::IMM, 2 },{ "CMP", &M::CMP, &M::IZX, 6 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "CPY", &M::CPY, &M::ZP0, 3 },{ "CMP", &M::CMP, &M::ZP0, 3 },{ "DEC", &M::DEC, &M::ZP0, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "INY", &M::INY, &M::IMP, 2 },{ "CMP", &M::CMP, &M::IMM, 2 },{ "DEX", &M::DEX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 2 },{ "CPY", &M::CPY, &M::ABS, 4 },{ "CMP", &M::CMP, &M::ABS, 4 },{ "DEC", &M::DEC, &M::ABS, 6 },{ "???", &M::XXX, &M::IMP, 6 },
        { "BNE", &M::BNE, &M::REL, 2 },{ "CMP", &M::CMP, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 4 },{ "CMP", &M::CMP, &M::ZPX, 4 },{ "DEC", &M::DEC, &M::ZPX, 6 },{ "???", &M::XXX, &M::IMP, 6 },{ "CLD", &M::CLD, &M::IMP, 2 },{ "CMP", &M::CMP, &M::ABY, 4 },{ "NOP", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 7 },{ "???", &M::NOP, &M::IMP, 4 },{ "CMP", &M::CMP, &M::ABX, 4 },{ "DEC", &M::DEC, &M::ABX, 7 },{ "???", &M::XXX, &M::IMP, 7 },
        { "CPX", &M::CPX, &M::IMM, 2 },{ "SBC", &M::SBC, &M::IZX, 6 },{ "???", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "CPX", &M::CPX, &M::ZP0, 3 },{ "SBC", &M::SBC, &M::ZP0, 3 },{ "INC", &M::INC, &M::ZP0, 5 },{ "???", &M::XXX, &M::IMP, 5 },{ "INX", &M::INX, &M::IMP, 2 },{ "SBC", &M::SBC, &M::IMM, 2 },{ "NOP", &M::NOP, &M::IMP, 2 },{ "???", &M::SBC, &M::IMP, 2 },{ "CPX", &M::CPX, &M::ABS, 4 },{ "SBC", &M::SBC, &M::ABS, 4 },{ "INC", &M::INC, &M::ABS, 6 },{ "???", &M::XXX, &M::IMP, 6 },
        { "BEQ", &M::BEQ, &M::REL, 2 },{ "SBC", &M::SBC, &M::IZY, 5 },{ "???", &M::XXX, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 8 },{ "???", &M::NOP, &M::IMP, 4 },{ "SBC", &M::SBC, &M::ZPX, 4 },{ "INC", &M::INC, &M::ZPX, 6 },{ "???", &M::XXX, &M::IMP, 6 },{ "SED", &M::SED, &M::IMP, 2 },{ "SBC", &M::SBC, &M::ABY, 4 },{ "NOP", &M::NOP, &M::IMP, 2 },{ "???", &M::XXX, &M::IMP, 7 },{ "???", &M::NOP, &M::IMP, 4 },{ "SBC", &M::SBC, &M::ABX, 4 },{ "INC", &M::INC, &M::ABX, 7 },{ "???", &M::XXX, &M::IMP, 7 },
    };

}

Chip6502::~Chip6502() {}

uint8_t Chip6502::read(uint16_t addr) 
{
    return bus->read(addr, false);
}

void Chip6502::write(uint16_t addr, uint8_t data) 
{
    bus->write(addr, data);
}

void Chip6502::clock() 
{
    if(cycles == 0)
    {
        opcode = read(pc);
        pc++;

        // Starting Cycles
        cycles = lookup[opcode].cycles;
        uint8_t additional_cycle1 = (this->*lookup[opcode].operate)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].addrmode)();

        cycles += (additional_cycle1 & additional_cycle2);
    }
    cycles--;
}

void Chip6502::reset() 
{
    
}

void Chip6502::irq() 
{
    
}

void Chip6502::nmi() 
{
    
}

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
    
}

uint8_t Chip6502::ZPX() 
{
    
}

uint8_t Chip6502::ZPY() 
{
    
}

uint8_t Chip6502::REL() 
{
    
}

uint8_t Chip6502::ABS() 
{
    
}

uint8_t Chip6502::ABX() 
{
    
}

uint8_t Chip6502::ABY() 
{
    
}

uint8_t Chip6502::IND() 
{
    
}

uint8_t Chip6502::IZX() 
{
    
}

uint8_t Chip6502::IZY() 
{
    
}



