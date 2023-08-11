#include "../include/mbc1chip.h"
#include "../include/tools.h"
#include <math.h>

void MBC1Chip::Open(FILE* fp, const ChipInfo& info)
{
	CartridgeChip::Open(fp, info);
	_rom.assign(info.numbytes, 0);
	_ram.assign(info.ramSize * 1024, 0);
	uint8* p = &_rom[0];
	fread(p, info.numbytes, 1, fp);
}

void MBC1Chip::Write(uint16 address, uint8 value)
{
	if (inRange(address, 0x0000, 0x1FFF)) {
		_ramEnabled = (value & 0x0A) != 0;
	}
	else if (inRange(address, 0x2000, 0x3FFF)) {
		if (value == 0x0) { _romBankNo = 0x1; }
		if (value == 0x20) { _romBankNo = 0x21; return; }
		if (value == 0x40) { _romBankNo = 0x41; return; }
		if (value == 0x60) { _romBankNo = 0x61; return; }
		_romBankNo = value & 0x1F;
	}
	else if (inRange(address, 0x4000, 0x5FFF)) {
		log("setting upper 2 bits is not implemented.");
	}
	else if (inRange(address, 0x6000, 0x7FFF)) {
		log("selecting ROM/RAM mode is not implemented.");
	}
	else if (inRange(address, 0xA000, 0xBFFF)) {
		if (!_ramEnabled) { return; }
		size_t offset = 0x2000 * _ramBankNo;
		size_t addressInRam = (address - 0xA000) + offset;
		if (addressInRam < _ram.size()) {
			_ram.at(addressInRam) = value;
		}
		else {
			log("attempt to write to RAM of %d with address %x", _ram.size(), address);
		}
	}
	else {
		CartridgeChip::Write(address, value);
	}
}

uint8 MBC1Chip::Read(uint16 address)
{
	if (inRange(address, 0x0000, 0x3FFF)) {
		return _rom.at(address);
	}
	else if (inRange(address, 0x4000, 0x7FFF)) {
		uint16 address_into_bank = address - 0x4000;
		int bank_offset = 0x4000 * _romBankNo;
		int address_in_rom = bank_offset + address_into_bank;
		return _rom.at(address_in_rom);
	}
	else if (inRange(address, 0xA000, 0xBFFF)) {
		size_t offset_into_ram = 0x2000 * _ramBankNo;
		size_t address_in_ram = (address - 0xA000) + offset_into_ram;
		if (address_in_ram < _ram.size()) {
			return _ram.at(address_in_ram);
		}
		else {
			log("attempt to read from RAM of %d with address %x", _ram.size(), address);
			return 0xFF;
		}
	}
	//else if (inRange(address, 0x0000, 0x1FFF)) {
	//	log("attempt to read from [0x0000-0x1FFF] on the MBC1.");
	//	return 0xFF;
	//}
	//else if (inRange(address, 0x2000, 0x3FFF)) {
	//	log("attempt to read from [0x2000-0x3FFF] on the MBC1.");
	//	return 0xFF;
	//}
	else {
		return CartridgeChip::Read(address);
	}
}
