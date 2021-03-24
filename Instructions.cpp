#include "headers/Chip6502.h"

uint8_t Chip6502::ADC() 
{
    // Get data that is going to be added to the accumulator
    fetch();
    // Add is done in 16bits
    uint16_t val = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

    // carry if only the my significant bit in high byte is 1
    setFlag(C, val > 255);

    // if val == 0 set zero flag
    setFlag(Z, (val & 0x00FF) == 0);

    // V = ~(a ^ m) & (a ^ r) plus most significant bit of low byte
    setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)val)) & 0x0080);
    
    // check most significant bit for negative flag
    setFlag(N, (val & 0x80));

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
    setFlag(Z, a == 0x00);
    // if most significant bit of low byte == 1
    setFlag(N, a & 0x80);
    return 1;
}

// Instruction: Arithmatic Shift Left
// Function: A = C <- (A << 1) <- 0
// Flags Out: C, Z, N
uint8_t Chip6502::ASL() 
{
    fetch();
    uint16_t val = (uint16_t)fetched << 1;
    setFlag(C, (val & 0xFF00) > 0);
    setFlag(Z, (val & 0x00FF) == 0x00);
    setFlag(N, val & 0x80);

    uint8_t temp = val & 0x00FF;

    if(lookup[opcode].addrmode == &Chip6502::IMP)
    {
        a = temp;
    }
    else
    {
        write(addr_abs, temp);
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

uint8_t Chip6502::BIT() 
{
    
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
    setFlag(C, false);
    return 0;
}

// Instruction: Clear Decimal Flag
// Function: D = 0
// Flags Out: none
uint8_t Chip6502::CLD() 
{
    setFlag(D, false);
    return 0;
}

// Instruction: Clear Interrupt Flag
// Function: I = 0
// Flags Out: none
uint8_t Chip6502::CLI() 
{
    setFlag(I, false);
    return 0;
}

// Instruction: Clear Overflow Flag
// Function: V = 0
// Flags Out: none
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

// Instruction: Push Accumulator to Stack
// Function: A -> Stack
// Flags Out: none
uint8_t Chip6502::PHA() 
{
    write(0x0100 + sp, a);
    sp--;
    return 0;
}

uint8_t Chip6502::PHP() 
{
    
}

// Instruction: Pop Accumulator off Stack
// Function: A <- Stack
// Flags Out: Z, N
uint8_t Chip6502::PLA() 
{
    sp++;
    a = read(0x0100 + sp);
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 0;
}

// Instruction: Pop Status Reguster off Stack
// Function: status <- Stack
// Flags Out: U
uint8_t Chip6502::PLP() 
{
    sp++;
    status = read(0x0100 + sp);
    setFlag(U, 1);
    return 0;
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
    setFlag(C, val & 255);
    setFlag(Z, (val & 0x00FF) == 0);
    setFlag(V, (val ^ (uint16_t)a) & (val ^ temp) & 0x0080);
    setFlag(N, (val & 0x80));
    a = val & 0x00FF;

    // SBC could require addition clock cycles
    return 1;

}

// Instruction: Set Carry Flag
// Function: C = 1
// Flags Out: none
uint8_t Chip6502::SEC() 
{
    setFlag(C, true);
    return 0;
}

// Instruction: Set Decimal Flag
// Function: D = 1
// Flags Out: none
uint8_t Chip6502::SED() 
{
    setFlag(D, true);
    return 0;
}

// Instruction: Set Interrupt Flag
// Function: I = 1
// Flags Out: none
uint8_t Chip6502::SEI() 
{
    setFlag(I, true);
    return 0;
}

// Instruction: Store Accumulator at Address
// Function: M = A
// Flags Out: none
uint8_t Chip6502::STA() 
{
    write(addr_abs, a);
    return 0;
}

// Instruction: Store X Register at Address
// Function: M = x
// Flags Out: none
uint8_t Chip6502::STX() 
{
    write(addr_abs, x);
    return 0;
}

// Instruction: Store Y Register at Address
// Function: M = y
// Flags Out: none
uint8_t Chip6502::STY() 
{
    write(addr_abs, y);
    return 0;
}

// Instruction: Transfer Accumulator to X Register
// Function: X = A
// Flags Out: Z, N
uint8_t Chip6502::TAX() 
{
    x = a;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer Accumulator to Y Register
// Function: Y = A
// Flags Out: Z, N
uint8_t Chip6502::TAY() 
{
    y = a;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}

// Instruction: Transfer Stack Pointer to X Register
// Function: X = SP
// Flags Out: Z, N
uint8_t Chip6502::TSX() 
{
    x = sp;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer X Register to Accumulator
// Function: A = X
// Flags Out: Z, N
uint8_t Chip6502::TXA() 
{
    a = x;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
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
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 0;
}


// Capture Illeage OpCodes
uint8_t Chip6502::XXX() 
{
    return 0;
}