#pragma once

#include <iostream>
#include "datatype.h"

struct ChipInfo
{
	int numbytes, romSize, ramSize;
};

class CartridgeChip
{
public:
	virtual ~CartridgeChip() {}

	virtual void Open(FILE* fp, const ChipInfo& info) { chipInfo = info; }

	virtual void Write(uint16 address, uint8 value) {}

	virtual uint8 Read(uint16 address) { return 0xFF; }

protected:
	ChipInfo chipInfo;
};

