CXX=g++
CXXFLAGS=-luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17 -Wall -Wno-unknown-pragmas
OBJS=Driver.o Chip2C02.o Bus.o Chip2C02.o Cartridge.o
SRC=*.cpp
TARGET_EXEC=Chip6502

$(TARGET_EXEC): $(OBJS)
	$(CXX) $(SRC) -o $(TARGET_EXEC) $(CXXFLAGS)

Driver.o: Driver.cpp
	g++ -c Driver.cpp

Chip6502.o: Chip6502.cpp
	g++ -c Chip6502.cpp

Bus.o: Bus.cpp
	g++ -c Bus.cpp

Chip2C02.o: Chip2C02.cpp
	g++ -c Chip2C02.cpp

Cartridge.o: Cartridge.cpp
	g++ -c Cartridge.cpp

clean:
	del *.o nes