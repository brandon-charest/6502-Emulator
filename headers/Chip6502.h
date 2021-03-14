#pragma once
#include <string>
// Forward declaration of Bus class to prevent
// circular inclusions
class Bus;

class Chip6502 
{
public:
    Chip6502();
    ~Chip6502();

    void ConnectBus(Bus *n) { bus = n; }

private:
    Bus *bus = nullptr;
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};
