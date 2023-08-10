#pragma once

#include <iostream>
#include "cartridgechip.h"

class RomOnlyChip : public CartridgeChip
{
public:
	void Open(FILE* fp, const ChipInfo& info) override;

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

private:
	uint8 _rom[0x8000];
};

