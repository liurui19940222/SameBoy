#include "../include/romonlychip.h"
#include "../include/tools.h"

void RomOnlyChip::Open(FILE* fp, const ChipInfo& info)
{
    CartridgeChip::Open(fp, info);
    uint8* p = _rom;
    fread(p, info.numbytes, 1, fp);
}

void RomOnlyChip::Write(uint16 address, uint8 value)
{
    log("attemp to write data %x to RomOnly chip at %x.", value, address);
}

uint8 RomOnlyChip::Read(uint16 address)
{
    return _rom[address];
}
