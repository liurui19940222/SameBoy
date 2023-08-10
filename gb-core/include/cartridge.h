#pragma once

#include "datatype.h"
#include "memory.h"
#include "cartridgechip.h"

struct GBCardHeader {
	int EntryPoint;
	char Logo[48];
	char Title[16];
	int16 NewLicenseeCode;
	int8 SGBFlag;
	uint8 Type;
	int8 ROMSize;
	int8 RAMSize;
	int8 DestinationCode;
	int8 OldLicenseeCode;
	int8 MaskROMVersionNumber;
	int8 HeaderCheckSum;
	int16 GlobalCheckSum;
};

class Cartridge : public IODevice
{
public:
	virtual ~Cartridge();

	bool Open(const char* romPath);

	void Print(GBCardHeader* header);

	bool Checksum(GBCardHeader* header);

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

private:
	CartridgeChip* _chip = nullptr;

	GBCardHeader _header;

	void _getRomRamSize(int& romSize, int& ramSize);
};

