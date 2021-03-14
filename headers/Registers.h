#pragma once
#include <cstdint>
// Registers
// https://wiki.nesdev.com/w/index.php/CPU_registers
uint8_t status = 0x00;  // Status Register
uint8_t pc = 0x00;      // Program Counter
uint8_t sp = 0x00;      // Stack Pointer
uint8_t a = 0x00;       // Accumulator
uint8_t x = 0x00;       // X Register
uint8_t y = 0x00;       // Y Register
