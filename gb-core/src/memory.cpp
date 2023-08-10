#include <cstring>
#include "../include/memory.h"
#include "../include/tools.h"

MemoryBus::MemoryBus()
{
	memset(_vRAM, 0, 0x2000);
	memset(_internalRAM, 0, 0x2000);
	memset(_oam, 0, 0xA0);
	memset(_highRAM, 0, 0x7F);
}

uint8 MemoryBus::ReadByte(uint16 address) {
	switch (address) {
	case MEM_IE:
	case MEM_IF:
		return _ioDevices[IOType::CPU]->Read(address);
	case MEM_DIV:
	case MEM_TIMA:
	case MEM_TMA:
	case MEM_TAC:
		return _ioDevices[IOType::Timer]->Read(address);
	case MEM_JOY:
		return _ioDevices[IOType::Joypad]->Read(address);
	case MEM_LCDC:
	case MEM_LY:
	case MEM_LYC:
	case MEM_LCD_STAT:
	case MEM_SCX:
	case MEM_SCY:
	case MEM_WNDX:
	case MEM_WNDY:
	case MEM_DMA:
	case MEM_BGP:
	case MEM_OBJP0:
	case MEM_OBJP1:
		return _ioDevices[IOType::PPU]->Read(address);
	case MEM_NR10:
	case MEM_NR11:
	case MEM_NR12:
	case MEM_NR13:
	case MEM_NR14:
	case MEM_NR21:
	case MEM_NR22:
	case MEM_NR23:
	case MEM_NR24:
	case MEM_NR30:
	case MEM_NR31:
	case MEM_NR32:
	case MEM_NR33:
	case MEM_NR34:
	case MEM_NR41:
	case MEM_NR42:
	case MEM_NR43:
	case MEM_NR44:
	case MEM_NR50:
	case MEM_NR51:
	case MEM_NR52:
		return _ioDevices[IOType::APU]->Read(address);
		break;
	default:
		if (inRange(address, 0, 0x7FFF) || inRange(address, 0xA000, 0xBFFF)) {
			return _ioDevices[IOType::Cartridge]->Read(address);
		}
		else if (inRange(address, 0x8000, 0x9FFF)) {
			return _vRAM[address - 0x8000];
		}
		else if (inRange(address, 0xC000, 0xDFFF)) {
			return _internalRAM[address - 0xC000];
		}
		else if (inRange(address, 0xFE00, 0xFE9F)) {
			return _oam[address - 0xFE00];
		}
		else if (inRange(address, 0xFF80, 0xFFFE)) {
			return _highRAM[address - 0xFF80];
		}
		else if (inRange(address, 0xFF30, 0xFF3F)) {
			return _ioDevices[IOType::APU]->Read(address);
		}
		else if (inRange(address, 0xFF01, 0xFF02)) {
			log("attemp to read data from serial space.");
			return 0xFF;
		}
		else if (inRange(address, 0xE000, 0xFDFF)) {
			log("attemp to read data from unusable [0xE000-0xFDFF] space.");
			return 0xFF;
		}
		else if (inRange(address, 0xFEA0, 0xFEFF)) {
			log("attemp to read data from unusable [0xFEA0-0xFEFF] space.");
			return 0xFF;
		}
		else if (inRange(address, 0xFF4C, 0xFF7F)) {
			log("attemp to read data from unusable [0xFF4C-0xFF7F] space.");
			return 0xFF;
		}
		else {
			log("attemp to read data from %x", address);
			return 0xFF;
		}
	}
}

void MemoryBus::WriteByte(uint16 address, uint8 value)
{
	switch (address) {
	case MEM_IE:
	case MEM_IF:
		_ioDevices[IOType::CPU]->Write(address, value);
		break;
	case MEM_DIV:
	case MEM_TIMA:
	case MEM_TMA:
	case MEM_TAC:
		_ioDevices[IOType::Timer]->Write(address, value);
		break;
	case MEM_JOY:
		_ioDevices[IOType::Joypad]->Write(address, value);
		break;
	case MEM_LY:
	case MEM_LYC:
	case MEM_LCD_STAT:
	case MEM_SCX:
	case MEM_SCY:
	case MEM_WNDX:
	case MEM_WNDY:
	case MEM_LCDC:
	case MEM_DMA:
	case MEM_BGP:
	case MEM_OBJP0:
	case MEM_OBJP1:
		_ioDevices[IOType::PPU]->Write(address, value);
		break;
	case MEM_NR10:
	case MEM_NR11:
	case MEM_NR12:
	case MEM_NR13:
	case MEM_NR14:
	case MEM_NR21:
	case MEM_NR22:
	case MEM_NR23:
	case MEM_NR24:
	case MEM_NR30:
	case MEM_NR31:
	case MEM_NR32:
	case MEM_NR33:
	case MEM_NR34:
	case MEM_NR41:
	case MEM_NR42:
	case MEM_NR43:
	case MEM_NR44:
	case MEM_NR50:
	case MEM_NR51:
	case MEM_NR52:
		_ioDevices[IOType::APU]->Write(address, value);
		break;
	default:
		if (inRange(address, 0, 0x7FFF) || inRange(address, 0xA000, 0xBFFF)) {
			_ioDevices[IOType::Cartridge]->Write(address, value);
		}
		else if (inRange(address, 0x8000, 0x9FFF)) {
			_vRAM[address - 0x8000] = value;
		}
		else if (inRange(address, 0xC000, 0xDFFF)) {
			_internalRAM[address - 0xC000] = value;
		}
		else if (inRange(address, 0xFE00, 0xFE9F)) {
			_oam[address - 0xFE00] = value;
		}
		else if (inRange(address, 0xFF80, 0xFFFE)) {
			_highRAM[address - 0xFF80] = value;
		}
		else if (inRange(address, 0xFF30, 0xFF3F)) {
			_ioDevices[IOType::APU]->Write(address, value);
		}
		else if (inRange(address, 0xFF01, 0xFF02)) {
			log("attemp to write data to serial space.");
		}
		else if (inRange(address, 0xE000, 0xFDFF)) {
			log("attemp to write data to unusable [0xE000-0xFDFF] space.");
		}
		else if (inRange(address, 0xFEA0, 0xFEFF)) {
			log("attemp to write data to unusable [0xFEA0-0xFEFF] space.");
		}
		else if (inRange(address, 0xFF4C, 0xFF7F)) {
			log("attemp to write data to unusable [0xFF4C-0xFF7F] space.");
		}
		else {
			log("attemp to write data at %x", address);
		}
		break;
	}
}

uint16 MemoryBus::Read2Byte(uint16 address) {
	uint8 high = ReadByte(address + 1);
	uint8 low = ReadByte(address);
	return (uint16)((high << 8) | low);
}

void MemoryBus::Write2Byte(uint16 address, uint16 value)
{
	WriteByte(address, (uint8)(value & 0xFF));
	WriteByte(address + 1, (uint8)(value >> 8));
}

void MemoryBus::SetBit(uint16 address, uint8 bitIndex, uint8 bitValue) {
	uint8 value = ReadByte(address);
	setBit(value, bitIndex, bitValue);
	WriteByte(address, value);
}

bool MemoryBus::CheckBit(uint16 address, uint8 bitIndex) {
	return checkBit(ReadByte(address), bitIndex);
}

void MemoryBus::SetIODevice(IOType type, IODevice* device) {
	_ioDevices[type] = device;
}
