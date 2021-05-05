#include "headers/Chip6502.h"
#include "headers/Bus.h"


Chip6502::Chip6502() 
{
   
    // I did not make this look up. Only modified it slightly to keep in line with my own implementation
    // Thanks to OneLoneCoder for spending literally days making this.

    // This Op Code Matrix is made to mirror the original table found below on pg 10.
    // http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf
    using M = Chip6502;
    lookup = 
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


bool Chip6502::complete()
{
    return cycles == 0;
}

uint8_t Chip6502::cpuRead(uint16_t addr) 
{
    return bus->cpuRead(addr, false);
}

void Chip6502::cpuWrite(uint16_t addr, uint8_t data) 
{
    bus->cpuWrite(addr, data);
}

// not clock cycle accurate
void Chip6502::clock() 
{
    if(cycles == 0)
    {
        opcode = cpuRead(pc);
        SetFlag(U, true);

        pc++;

        // Starting Cycles
        cycles = lookup[opcode].cycles;
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        // check if both op function and addr mode function require addition cycles
        // update cycles 
        cycles += (additional_cycle1 & additional_cycle2);
        SetFlag(U, true);

    }
    cycles--;
}

uint8_t Chip6502::fetch()
{
    if(!(lookup[opcode].addrmode == &Chip6502::IMP))
    {
        fetched = cpuRead(addr_abs);
    }
    return fetched;
}

void Chip6502::reset() 
{
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    status = 0x00 | U;

    // Address around 0x00 is saved for user programmable states
    addr_abs = 0xFFFC;
    uint16_t lo = cpuRead(addr_abs + 0);
    uint16_t hi = cpuRead(addr_abs + 1);

    pc = (hi << 8) | lo;
    addr_abs = 0x0000;
    addr_rel = 0x0000;
    fetched = 0x00;

    cycles = 8;
}

void Chip6502::irq() 
{
    if(GetFlag(I) == 0)
    {
        cpuWrite(sp_mem_offset, (pc >> 8) & 0x00FF);
        sp--;
        cpuWrite(sp_mem_offset, pc & 0x00FF);
        sp--;

        SetFlag(B, 0);
        SetFlag(U, 1);
        SetFlag(I, 1);
        cpuWrite(sp_mem_offset, status);
        sp--;


        addr_abs = 0xFFFE;
        uint16_t lo = cpuRead(addr_abs + 0);
        uint16_t hi = cpuRead(addr_abs + 1);
        pc = (hi << 8) | lo;

        cycles = 7;
    }
}

void Chip6502::nmi() 
{
    cpuWrite(sp_mem_offset, (pc >> 8) & 0x00FF);
    sp--;
    cpuWrite(sp_mem_offset, pc & 0x00FF);
    sp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    cpuWrite(sp_mem_offset, status);
    sp--;


    addr_abs = 0xFFFA;
    uint16_t lo = cpuRead(addr_abs + 0);
    uint16_t hi = cpuRead(addr_abs + 1);
    pc = (hi << 8) | lo;

    cycles = 8;
}


uint8_t Chip6502::GetFlag(StatusFlags f)
{
	return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of the status register
void Chip6502::SetFlag(StatusFlags f, bool v)
{
	if (v)
	{
        status |= f;
    }	
	else
    {
		status &= ~f;
    }
}


// Credit for disassembler goes to OneLoneCoder
std::map<uint16_t, std::string> Chip6502::disassemble(uint16_t nStart, uint16_t nStop) 
{
    uint32_t addr = nStart;
    uint8_t val = 0x00;
    uint8_t lo = 0x00;
    uint8_t hi = 0x00;
    uint16_t line_addr = 0;
    std::map<uint16_t, std::string> mapLines;

    // convert variable into hex strings, for convience
    auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

    while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cpuRead(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode == &Chip6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::IMM)
		{
			val = bus->cpuRead(addr, true); addr++;
			sInst += "#$" + hex(val, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::ZP0)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;												
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::ZPX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::ZPY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::IZX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::IZY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::ABS)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::ABX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::ABY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::IND)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &Chip6502::REL)
		{
			val = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex(val, 2) + " [$" + hex(addr + val, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}

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
    addr_abs = cpuRead(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Chip6502::ZPX() 
{
    addr_abs = (cpuRead(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Chip6502::ZPY() 
{
    addr_abs = (cpuRead(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t Chip6502::REL() 
{
    addr_rel = cpuRead(pc);
    pc++;

    if(addr_rel & 0x80)
    {
        addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t Chip6502::ABS() 
{
    uint16_t lo = cpuRead(pc);
    pc++;
    uint16_t hi = cpuRead(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t Chip6502::ABX() 
{
    uint16_t lo = cpuRead(pc);
    pc++;
    uint16_t hi = cpuRead(pc);
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
    uint16_t lo = cpuRead(pc);
    pc++;
    uint16_t hi = cpuRead(pc);
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
    uint16_t ptr_lo = cpuRead(pc);
    pc++;
    uint16_t ptr_hi = cpuRead(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;
    
    // The hardware bug was known and accounted for by NES game creaters and needs to be replicated.
    // The IND(xxFF) will fail and instead grab address 00xx instead of xx+1
    if(ptr_lo == 0x00FF)
    {
        addr_abs = (cpuRead(ptr & 0xFF00) << 8) | cpuRead(ptr + 0);
    }
    else
    {
        addr_abs = (cpuRead(ptr + 1) << 8) | cpuRead(ptr + 0);
    }


    return 0;
}

uint8_t Chip6502::IZX() 
{
    uint16_t t = cpuRead(pc);
    pc++;

    uint16_t lo = cpuRead((uint16_t)(t + (uint16_t)x) & 0x00ff);
    uint16_t hi = cpuRead((uint16_t)(t + (uint16_t)x + 1) & 0x00ff);

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t Chip6502::IZY() 
{
    uint16_t t = cpuRead(pc);
    pc++;

    uint16_t lo = cpuRead(t & 0x00FF);
    uint16_t hi = cpuRead((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if((addr_abs & 0xFF00) != (hi << 8))
    {
        return 1;
    }
    return 0;
}




uint8_t Chip6502::ADC() 
{
    // Get data that is going to be added to the accumulator
    fetch();
    // Add is done in 16bits
    uint16_t val = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

    // carry if only the my significant bit in high byte is 1
    SetFlag(C, val > 255);

    // if val == 0 set zero flag
    SetFlag(Z, (val & 0x00FF) == 0);

    // V = ~(a ^ m) & (a ^ r) plus most significant bit of low byte
    SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)val)) & 0x0080);
    
    // check most significant bit for negative flag
    SetFlag(N, (val & 0x80));

    // load result into accumulator
    a = val & 0x00FF;

    // ADC could require addition clock cycles
    return 1;
}




// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t Chip6502::AND() 
{
    fetch();
    a = a & fetched;
    // if all bits 0
    SetFlag(Z, a == 0x00);
    // if most significant bit of low byte == 1
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Arithmatic Shift Left
// Function: A = C <- (A << 1) <- 0
// Flags Out: C, Z, N
uint8_t Chip6502::ASL() 
{
    fetch();
    uint16_t val = (uint16_t)fetched << 1;
    SetFlag(C, (val & 0xFF00) > 0);
    SetFlag(Z, (val & 0x00FF) == 0x00);
    SetFlag(N, val & 0x80);

    uint8_t temp = val & 0x00FF;

    if(lookup[opcode].addrmode == &Chip6502::IMP)
    {
        a = temp;
    }
    else
    {
        cpuWrite(addr_abs, temp);
    }
    return 0;
}

// Instruction: Branch if Carry Clear
// Function: if (C == 0) { pc = address}
// Flags Out: none
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

// Instruction: Branch if Carry Set
// Function: if (C == 1) { pc = address}
// Flags Out: none
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

// Instruction: Branch if Equal
// Function: if (Z == 1) { pc = address}
// Flags Out: none
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


// Instruction: Test if one or more bits are set at target location
// Function: 
// Flags Out: Z, V, N
uint8_t Chip6502::BIT() 
{
    fetch();
    uint8_t temp = a & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(V, fetched & (1 << 7));
	SetFlag(N, fetched & (1 << 6));
	return 0;
}

// Instruction: Branch if Negative
// Function: if (N == 1) { pc = address}
// Flags Out: none
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

// Instruction: Branch if Not Equal
// Function: if (Z == 0) { pc = address}
// Flags Out: none
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

// Instruction: Branch if Positive
// Function: if (N == 0) { pc = address}
// Flags Out: none
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

// Instruction: Break
// Function: Program Interrupt
// Flags Out: none
uint8_t Chip6502::BRK() 
{
   pc++;

    SetFlag(I, 1);
    cpuWrite(sp_mem_offset, (pc >> 8) & 0x00FF);
    sp--;
    cpuWrite(sp_mem_offset, pc & 0x00FF);
    sp--;

    SetFlag(B, 1);
    cpuWrite(sp_mem_offset, status);
    sp--;
    SetFlag(B, 0);

    pc = (uint16_t)cpuRead(0xFFFE) | ((uint16_t)cpuRead(0xFFFF) << 8);
    return 0;
}

// Instruction: Branch if Overflow CLear
// Function: if (V == 0) { pc = address}
// Flags Out: none
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

// Instruction: Branch if Overflow CLear
// Function: if (V == 1) { pc = address}
// Flags Out: none
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

// Instruction: Clear Carry Flag
// Function: C = 0
// Flags Out: none
uint8_t Chip6502::CLC() 
{
    SetFlag(C, false);
    return 0;
}

// Instruction: Clear Decimal Flag
// Function: D = 0
// Flags Out: none
uint8_t Chip6502::CLD() 
{
    SetFlag(D, false);
    return 0;
}

// Instruction: Clear Interrupt Flag
// Function: I = 0
// Flags Out: none
uint8_t Chip6502::CLI() 
{
    SetFlag(I, false);
    return 0;
}

// Instruction: Clear Overflow Flag
// Function: V = 0
// Flags Out: none
uint8_t Chip6502::CLV() 
{
    SetFlag(V, false);
    return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M,    Z <- (A - M) == 0
// Flags Out:   C, Z, N
uint8_t Chip6502::CMP() 
{
    fetch();
    uint16_t temp = (uint16_t)a - (uint16_t)fetched;
    SetFlag(C, a >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: Compare X Register
// Function:    C <- X >= M,    Z <- (X - M) == 0
// Flags Out:   C, Z, N
uint8_t Chip6502::CPX() 
{
    fetch();
    uint16_t temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: Compare Y Register
// Function:    C <- Y >= M,    Z <- (Y - M) == 0
// Flags Out:   C, Z, N
uint8_t Chip6502::CPY() 
{
    fetch();
    uint16_t temp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: Decrement Value at Memory Address
// Function:    M = M - 1
// Flags Out:   Z, N
uint8_t Chip6502::DEC() 
{
    fetch();
    uint8_t temp = fetched - 1;
    cpuWrite(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   Z, N
uint8_t Chip6502::DEX() 
{
     x--;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   Z, N
uint8_t Chip6502::DEY() 
{
    y--;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Bitwise Logic XOR
// Function:    A = A ^ M
// Flags Out:   Z, N
uint8_t Chip6502::EOR() 
{
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Increment Value at Memory Address
// Function:    M = M + 1
// Flags Out:   Z, N
uint8_t Chip6502::INC() 
{
    fetch();
    uint8_t temp = fetched + 1;
    cpuWrite(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   Z, N
uint8_t Chip6502::INX() 
{
    x++;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   Z, N
uint8_t Chip6502::INY() 
{
    y++;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Jump to memory location
// Function: pc = address
// Flags Out: None
uint8_t Chip6502::JMP() 
{
    pc = addr_abs;
    return 0;
}

// Instruction: Jump to sub-routine
// Function: pc -> stack, pc = address
// Flags Out: None
uint8_t Chip6502::JSR() 
{
    pc--;
    cpuWrite(sp_mem_offset, (pc >> 8) & 0x00FF);
    sp--;
    cpuWrite(sp_mem_offset, pc & 0x00FF);
    sp--;
    pc = addr_abs;
    return 0;
}

// Instruction: Load Accumulator
// Function: A = M
// Flags Out: Z, N
uint8_t Chip6502::LDA() 
{
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Load X Register
// Function: X = M
// Flags Out: Z, N
uint8_t Chip6502::LDX() 
{
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 1;
}

// Instruction: Load Y Register
// Function: Y = M
// Flags Out: Z, N
uint8_t Chip6502::LDY() 
{
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 1;
}

// Instruction: Shift one bit right
// Function: m = m >> 1
// Flags Out: C, Z, N
uint8_t Chip6502::LSR() 
{
    fetch();
    uint8_t temp = fetched >> 1;

    SetFlag(C, fetched & 0x0001);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);

    if(lookup[opcode].addrmode == &Chip6502::IMP)
    {
        a = temp & 0x00FF;
    }
    else
    {
        cpuWrite(addr_abs, temp & 0x00FF);
    }
    return 0;
}

// Instruction: No Operation
// Function: 
// Flags Out: None
uint8_t Chip6502::NOP() 
{
    switch (opcode)
    {
    case 0x1C:
    case 0x3C:
    case 0x5c:
        return 1;
    
    default:
        break;
    }
    return 0;
}

// Instruction: Bitwise Logic OR
// Function: A = A | M
// Flags Out: Z, N
uint8_t Chip6502::ORA() 
{
    fetch();
    a = a | fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Push Accumulator to Stack
// Function: A -> Stack
// Flags Out: none
uint8_t Chip6502::PHA() 
{
    cpuWrite(sp_mem_offset, a);
    sp--;
    return 0;
}

// Instruction: Push Status Register to Stack
// Function: status -> Stack
// Flags Out: B, U
uint8_t Chip6502::PHP() 
{
    cpuWrite(sp_mem_offset, status | B | U);
    SetFlag(B, 0);
    SetFlag(U, 0);
    sp--;
    return 0;
}

// Instruction: Pop Accumulator off Stack
// Function: A <- Stack
// Flags Out: Z, N
uint8_t Chip6502::PLA() 
{
    sp++;
    a = cpuRead(sp_mem_offset);
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// Instruction: Pop Status Register off Stack
// Function: status <- Stack
// Flags Out: U
uint8_t Chip6502::PLP() 
{
    sp++;
    status = cpuRead(sp_mem_offset);
    SetFlag(U, 1);
    return 0;
}

// Instruction: Rotate bits at address to the left
// Function: m << d
// Flags Out: C, Z, N
uint8_t Chip6502::ROL() 
{
    fetch();
	uint16_t temp = (uint16_t)(fetched << 1) | GetFlag(C);

	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	if (lookup[opcode].addrmode == &Chip6502::IMP)
	{	
        a = temp & 0x00FF;
    }
	else
    {
		cpuWrite(addr_abs, temp & 0x00FF);
    }
	return 0;
}

// Instruction: Rotate bits at address to the left
// Function: m >> d
// Flags Out: C, Z, N
uint8_t Chip6502::ROR() 
{
    fetch();
	uint16_t temp = (uint16_t)(fetched << 7) | (fetched >> 1);

	SetFlag(C, temp & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	if (lookup[opcode].addrmode == &Chip6502::IMP)
	{	
        a = temp & 0x00FF;
    }
	else
    {
		cpuWrite(addr_abs, temp & 0x00FF);
    }
	return 0;
}

// Instruction: Return from Interrupt
// Function: 
// Flags Out: none
uint8_t Chip6502::RTI() 
{
    sp++;
    
    status = cpuRead(0x100 + sp);
    status &= ~B;
    status &= ~U;

    sp++;
    uint16_t mem = (uint16_t)cpuRead(sp_mem_offset);
    pc = mem;
    sp++;
    pc |= mem << 8;
    return 0;
}

// Instruction: Return from Sub-Routine
// Function: 
// Flags Out: none
uint8_t Chip6502::RTS() 
{
    sp++;
	pc = (uint16_t)cpuRead(sp_mem_offset);

	sp++;
	pc |= (uint16_t)cpuRead(sp_mem_offset) << 8;
	
	pc++;
	return 0;
}

uint8_t Chip6502::SBC() 
{
    // subtraction: 
    // A = A - M - (C - 1)
    // A = A + -1(M - (C - 1))
    // A = A + (-M) + 1 + C
    // A: accumulator, M: memory contents, C: carry

    fetch();

    // invert M
    uint16_t temp = ((uint16_t)fetched) ^ 0x00FF;


    // basically same as ADC
    uint16_t val = (uint16_t)a + temp + (uint16_t)GetFlag(C);    
    SetFlag(C, val & 255);
    SetFlag(Z, (val & 0x00FF) == 0);
    SetFlag(V, (val ^ (uint16_t)a) & (val ^ temp) & 0x0080);
    SetFlag(N, (val & 0x80));
    a = val & 0x00FF;

    // SBC could require addition clock cycles
    return 1;

}

// Instruction: Set Carry Flag
// Function: C = 1
// Flags Out: none
uint8_t Chip6502::SEC() 
{
    SetFlag(C, true);
    return 0;
}

// Instruction: Set Decimal Flag
// Function: D = 1
// Flags Out: none
uint8_t Chip6502::SED() 
{
    SetFlag(D, true);
    return 0;
}

// Instruction: Set Interrupt Flag
// Function: I = 1
// Flags Out: none
uint8_t Chip6502::SEI() 
{
    SetFlag(I, true);
    return 0;
}

// Instruction: Store Accumulator at Address
// Function: M = A
// Flags Out: none
uint8_t Chip6502::STA() 
{
    cpuWrite(addr_abs, a);
    return 0;
}

// Instruction: Store X Register at Address
// Function: M = x
// Flags Out: none
uint8_t Chip6502::STX() 
{
    cpuWrite(addr_abs, x);
    return 0;
}

// Instruction: Store Y Register at Address
// Function: M = y
// Flags Out: none
uint8_t Chip6502::STY() 
{
    cpuWrite(addr_abs, y);
    return 0;
}

// Instruction: Transfer Accumulator to X Register
// Function: X = A
// Flags Out: Z, N
uint8_t Chip6502::TAX() 
{
    x = a;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer Accumulator to Y Register
// Function: Y = A
// Flags Out: Z, N
uint8_t Chip6502::TAY() 
{
    y = a;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Transfer Stack Pointer to X Register
// Function: X = SP
// Flags Out: Z, N
uint8_t Chip6502::TSX() 
{
    x = sp;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer X Register to Accumulator
// Function: A = X
// Flags Out: Z, N
uint8_t Chip6502::TXA() 
{
    a = x;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// Instruction: Transfer X Register to Stack Pointer
// Function: SP = X
// Flags Out: none
uint8_t Chip6502::TXS() 
{
    sp = x;
    return 0;
}

// Instruction: Transfer Y Register to Stack Accumulator
// Function: A = Y
// Flags Out: Z, N
uint8_t Chip6502::TYA() 
{
    a = y;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}


// Capture Illeage OpCodes
uint8_t Chip6502::XXX() 
{
    return 0;
}


