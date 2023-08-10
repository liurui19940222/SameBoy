#pragma once

#include <vector>
#include "romonlychip.h"

using namespace std;

class MBC1Chip : public CartridgeChip
{
public:
	void Open(FILE* fp, const ChipInfo& info) override;

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

private:
	vector<uint8> _rom;

	vector<uint8> _ram;

	bool _ramEnabled = false;

	int _romBankNo = 1, _ramBankNo = 0;
};

