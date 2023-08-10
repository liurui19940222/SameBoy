#pragma once

#include <map>
#include "datatype.h"

const int MEM_LY = 0xFF44;
const int MEM_LYC = 0xFF45;
const int MEM_LCD_STAT = 0xFF41;
const int MEM_LCDC = 0xFF40;
const int MEM_SCX = 0xFF43;
const int MEM_SCY = 0xFF42;
const int MEM_WNDX = 0xFF4B;
const int MEM_WNDY = 0xFF4A;
const int MEM_DMA = 0xFF46;
const int MEM_JOY = 0xFF00;
const int MEM_BGP = 0xFF47;
const int MEM_OBJP0 = 0xFF48;
const int MEM_OBJP1 = 0xFF49;
const int MEM_OAM_BEGIN = 0xFE00;
const int MEM_OAM_END = 0xFE9F;
const int MEM_IE = 0xFFFF;
const int MEM_IF = 0xFF0F;
const int MEM_DIV = 0xFF04;
const int MEM_TIMA = 0xFF05;
const int MEM_TMA = 0xFF06;
const int MEM_TAC = 0xFF07;
const int MEM_NR10 = 0xFF10;
const int MEM_NR11 = 0xFF11;
const int MEM_NR12 = 0xFF12;
const int MEM_NR13 = 0xFF13;
const int MEM_NR14 = 0xFF14;
const int MEM_NR21 = 0xFF16;
const int MEM_NR22 = 0xFF17;
const int MEM_NR23 = 0xFF18;
const int MEM_NR24 = 0xFF19;
const int MEM_NR30 = 0xFF1A;
const int MEM_NR31 = 0xFF1B;
const int MEM_NR32 = 0xFF1C;
const int MEM_NR33 = 0xFF1D;
const int MEM_NR34 = 0xFF1E;
const int MEM_NR41 = 0xFF20;
const int MEM_NR42 = 0xFF21;
const int MEM_NR43 = 0xFF22;
const int MEM_NR44 = 0xFF23;
const int MEM_NR50 = 0xFF24;
const int MEM_NR51 = 0xFF25;
const int MEM_NR52 = 0xFF26;

enum class InterFlags : uint8 {
	VBlank = 0,
	LCDSTAT = 1,
	Timer = 2,
	Serial = 3,
	Joypad = 4
};

enum class IOType {
	CPU,
	PPU,
	APU,
	Timer,
	Joypad,
	Cartridge
};

class IODevice
{
public:
	virtual void Write(uint16 address, uint8 value) {}
	virtual uint8 Read(uint16 address) { return 0xFF; }
};

class MemoryBus
{
public:
	MemoryBus();

	uint8 ReadByte(uint16 address);

	uint16 Read2Byte(uint16 address);

	void WriteByte(uint16 address, uint8 value);

	void Write2Byte(uint16 address, uint16 value);

	void SetBit(uint16 address, uint8 bitIndex, uint8 bitValue);

	bool CheckBit(uint16 address, uint8 bitIndex);

	void SetIODevice(IOType type, IODevice* device);

private:
	uint8 _internalRAM[0x2000];

	uint8 _oam[0xA0];

	uint8 _highRAM[0x7F];

	uint8 _vRAM[0x2000];

	std::map<IOType, IODevice*> _ioDevices;
};

