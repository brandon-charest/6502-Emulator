#pragma once
#include <string>
#include <vector>
#include "StatusFlags.h"
#include "Registers.h"
// Forward declaration of Bus class to prevent
// circular inclusions
class Bus;
enum StatusFlags;

class Chip6502 
{
public:
    Chip6502();
    ~Chip6502();

    // CPU Connection
    void ConnectBus(Bus *n) { bus = n; }

    void clock();
    void reset();
    void irq(); // Interrupt request signal
    void nmi(); // Non maskable Interrupt request signal

    uint8_t fetch();
    uint8_t fetched = 0x00;

    uint16_t addr_abs = 0x0000;
    uint16_t addr_rel = 0x0000;
    uint8_t opcode = 0x00; // current opcode
    uint8_t cycles = 0; // Cycles left in instruction

private:
    Bus *bus = nullptr;
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    // functions to access status register
    uint8_t GetFlag(StatusFlags flag);
    void setFlag(StatusFlags flag, bool val);


    // Structure of the opcode matrix table
    // Each entrry contains:
    //      Pneumonic               : Text representation of the instruction
    //      Opdode Function         : Function pointer to opcode
    //      Opcode Address Mode     : Function pointer to addressing mode used by instruction
    //      Cycle Count             : Interger representation of number of clock cycles needed to perform instruction
   struct INSTRUCTION
	{
		std::string name;		
        uint8_t (Chip6502::*operate )(void) = nullptr;
		uint8_t (Chip6502::*addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	static const std::vector<INSTRUCTION> lookup;

private:
    // Addressing Modes *****************************************
    uint8_t IMP();
    uint8_t IMM();
    uint8_t ZP0();
    uint8_t ZPX();
    uint8_t ZPY();
    uint8_t REL();
    uint8_t ABS();
    uint8_t ABX();
    uint8_t ABY();
    uint8_t IND();
    uint8_t IZX();
    uint8_t IZY();

private:
    // Opcodes****************************************************
    // http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf
    // Instruction Set
    uint8_t ADC(); // "Add Memory to Accumulator with Carry"
    uint8_t AND(); // "'AND' Memory with Accumulator"
    uint8_t ASL(); // "Shit Left One Bit (Memory or Accumulator)"

    uint8_t BCC(); // "Branch on Carry Clear"
    uint8_t BCS(); // "Branch on Carry Set"
    uint8_t BEQ(); // "Branch on Result Zero"
    uint8_t BIT(); // "Test Bits in Memory with Accumulator"
    uint8_t BMI(); // "Branch on Result Minus"
    uint8_t BNE(); // "Branch on Result not Zero"
    uint8_t BPL(); // "Branch on Result Plus"
    uint8_t BRK(); // "Force Break"
    uint8_t BVC(); // "Branch on Overflow Clear"
    uint8_t BVS(); // "Branch on Overflow Set"

    uint8_t CLC(); // "Clear Carry Flag"
    uint8_t CLD(); // "Clear Decimal Mode"
    uint8_t CLI(); // "Clear Interrupt Disable Bit"
    uint8_t CLV(); // "Clear Overflow Flag"
    uint8_t CMP(); // "Compare Memory and Accumulator"
    uint8_t CPX(); // "Compare Memory and Index X"
    uint8_t CPY(); // "Compare Memory and Index Y"

    uint8_t DEC(); // "Decrement Memory by One"
    uint8_t DEX(); // "Decrement Index X by One"
    uint8_t DEY(); // "Decrement Index Y by One"

    uint8_t EOR(); // "'XOR' Memory with Accumulator"

    uint8_t INC(); // "Increment Memory by One"
    uint8_t INX(); // "Increment Index X by One"
    uint8_t INY(); // "Increment Index Y by One"

    uint8_t JMP(); // "Jump to New Location"
    uint8_t JSR(); // "Jump to New Location Saving Return Address"

    uint8_t LDA(); // "Load Acuumulator with Memory"
    uint8_t LDX(); // "Load Index X with Memory"
    uint8_t LDY(); // "Load Index Y with Memory"
    uint8_t LSR(); // "Shift One Bit Right (Memory or Accumulator)"

    uint8_t NOP(); // "No Opperation"

    uint8_t ORA(); // ""OR" Memory with Accumulator"

    uint8_t PHA(); // "Push Accumulator on Stack"
    uint8_t PHP(); // "Push Processor Status on Stack"
    uint8_t PLA(); // "Pull Accumulator from Stack"
    uint8_t PLP(); // "Pull Processor Status from Stack"

    uint8_t ROL(); // "Rotate One Bit Left (Memory or Accumulator)"
    uint8_t ROR(); // "Rotate One Bit Right (Memory or Accumulator)"
    uint8_t RTI(); // "Return from Interrupt"
    uint8_t RTS(); // "Return from Subroutine"

    uint8_t SBC(); // "Subtract Memory from Accumulator with Borrow"
    uint8_t SEC(); // "Set Carry Flag"
    uint8_t SED(); // "Set Decimal Mode"
    uint8_t SEI(); // "Set Interrupt Disable Status"
    uint8_t STA(); // "Store Accumulator in Memory"
    uint8_t STX(); // "Store Index X in Memory"
    uint8_t STY(); // "Store Index Y in Memory"

    uint8_t TAX(); // "Transfer Accumulator to Index X"
    uint8_t TAY(); // "Transfer Accumulator to Index Y"
    uint8_t TSX(); // "Transfer Stack Pointer to Index X"
    uint8_t TXA(); // "Transfer Index X to Accumulator"
    uint8_t TXS(); // "Transfer Index X to Stack Register"
    uint8_t TYA(); // "Transfer Index Y to Accumulator"

    uint8_t XXX(); // Illegal OpCode
};


