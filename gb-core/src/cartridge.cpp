#include <iostream>
#include "../include/cartridge.h"
#include "../include/tools.h"
#include "../include/romonlychip.h"
#include "../include/mbc1chip.h"

Cartridge::~Cartridge()
{
    if (_chip) delete _chip;
}

bool Cartridge::Open(const char* romPath) {
    FILE* fp;
    GBCardHeader header;
    int numbytes;
    if ((fp = fopen(romPath, "rb")) == NULL) {
        return false;
    }
    // if (fopen_s(&fp, romPath, "rb") != 0) {
    //     return false;
    // }
    fseek(fp, 0x100, SEEK_SET);
    fread(&header, sizeof(GBCardHeader), 1, fp);
    fseek(fp, 0, SEEK_SET);

    fseek(fp, 0L, SEEK_END);
    numbytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    Print(&header);

    switch (header.Type)
    {
    case 0x00:
        _chip = new RomOnlyChip();
        break;
    case 0x01:
        _chip = new MBC1Chip();
        break;
    default:
        log("unknown chip type %d", header.Type);
        return false;
    }

    int romSize, ramSize;
    _getRomRamSize(romSize, ramSize);
    _chip->Open(fp, ChipInfo{ numbytes, romSize, ramSize });

    fclose(fp);

    return true;
}

bool Cartridge::Checksum(GBCardHeader* header) {
    int8* p = (int8*)header;
    int8 sum = 25;
    for (int i = 52; i < 78; ++i) {
        sum += *(p + i);
    }
    return sum == 0;
}

void Cartridge::Write(uint16 address, uint8 value) {
    _chip->Write(address, value);
}

uint8 Cartridge::Read(uint16 address) {
    return _chip->Read(address);
}

void Cartridge::Print(GBCardHeader* header) {
    const char* type;
    const char* romSize;
    const char* ramSize;
    const char* destCode;
    bool checkSum;
    switch (header->Type)
    {
    case 0x00:
        type = "ROM ONLY";
        break;
    case 0x01:
        type = "MBC1";
        break;
    case 0x02:
        type = "MBC1+RAM";
        break;
    case 0x03:
        type = "MBC1+RAM+BATTERY";
        break;
    case 0x05:
        type = "MBC2";
        break;
    case 0x06:
        type = "MBC2+BATTERY";
        break;
    case 0x08:
        type = "ROM+RAM";
        break;
    case 0x09:
        type = "ROM+RAM+BATTERY";
        break;
    case 0x0B:
        type = "MMM01";
        break;
    case 0x0C:
        type = "MMM01+RAM";
        break;
    case 0x0D:
        type = "MMM01+RAM+BATTERY";
        break;
    case 0x0F:
        type = "MBC3+TIMER+BATTERY";
        break;
    case 0x10:
        type = "MBC3+TIMER+RAM+BATTERY";
        break;
    case 0x11:
        type = "MBC3";
        break;
    case 0x12:
        type = "MBC3+RAM";
        break;
    case 0x13:
        type = "MBC3+RAM+BATTERY";
        break;
    case 0x15:
        type = "MBC4";
        break;
    case 0x16:
        type = "MBC4+RAM";
        break;
    case 0x17:
        type = "MBC4+RAM+BATTERY";
        break;
    case 0x19:
        type = "MBC5";
        break;
    case 0x1A:
        type = "MBC5+RAM";
        break;
    case 0x1B:
        type = "MBC5+RAM+BATTERY";
        break;
    case 0x1C:
        type = "MBC5+RUMBLE";
        break;
    case 0x1D:
        type = "MBC5+RUMBLE+RAM";
        break;
    case 0x1E:
        type = "MBC5+RUMBLE+RAM+BATTERY";
        break;
    case 0xFC:
        type = "POCKET CAMERA";
        break;
    case 0xFD:
        type = "BANDAI TAMA5";
        break;
    case 0xFE:
        type = "HuC3";
        break;
    case 0xFF:
        type = "HuC1+RAM+BATTERY";
        break;
    default:
        type = NULL;
        break;
    }

    switch (header->ROMSize) {
    case 0x00:
        romSize = "32KByte (no ROM banking)";
        break;
    case 0x01:
        romSize = "64KByte (4 banks)";
        break;
    case 0x02:
        romSize = "128KByte (8 banks)";
        break;
    case 0x03:
        romSize = "256KByte (16 banks)";
        break;
    case 0x04:
        romSize = "512KByte (32 banks)";
        break;
    case 0x05:
        romSize = "1MByte (64 banks)  - only 63 banks used by MBC1";
        break;
    case 0x06:
        romSize = "2MByte (128 banks) - only 125 banks used by MBC1";
        break;
    case 0x07:
        romSize = "4MByte (256 banks)";
        break;
    case 0x52:
        romSize = "1.1MByte (72 banks)";
        break;
    case 0x53:
        romSize = "1.2MByte (80 banks)";
        break;
    case 0x54:
        romSize = "1.5MByte (96 banks)";
        break;
    default:
        romSize = NULL;
        break;
    }

    switch (header->RAMSize) {
    case 0x00:
        ramSize = "None";
        break;
    case 0x01:
        ramSize = "2 KBytes";
        break;
    case 0x02:
        ramSize = "8 Kbytes";
        break;
    case 0x03:
        ramSize = "32 KBytes (4 banks of 8KBytes each)";
        break;
    default:
        ramSize = NULL;
        break;
    }

    switch (header->DestinationCode) {
    case 0x00:
        destCode = "Japanese";
        break;
    case 0x01:
        destCode = "Non-Japanese";
        break;
    default:
        destCode = NULL;
        break;
    }

    checkSum = Checksum(header);

    log("EntryPoint:%d\nTitle:%s\nNewLisenseeCode:%d\nSGBFlag:%d\nType:%s\nROMSize:%s\nRAMSize:%s\nDestinationCode:%s\nOldLicensessCode:%d\nMaskROMVersionNumber:%d\nHeaderCheckSum:%d\nGlobalCheckSum:%d\nCheckSumOK:%d\n",
        header->EntryPoint,
        header->Title,
        header->NewLicenseeCode,
        header->SGBFlag,
        type,
        romSize,
        ramSize,
        destCode,
        header->OldLicenseeCode,
        header->MaskROMVersionNumber,
        header->HeaderCheckSum,
        header->GlobalCheckSum,
        checkSum);
}

void Cartridge::_getRomRamSize(int& romSize, int& ramSize)
{
    switch (_header.ROMSize) {
    case 0x00:
        romSize = 32;
        break;
    case 0x01:
        romSize = 64;
        break;
    case 0x02:
        romSize = 128;
        break;
    case 0x03:
        romSize = 256;
        break;
    case 0x04:
        romSize = 512;
        break;
    case 0x05:
        romSize = 1024;
        break;
    case 0x06:
        romSize = 2048;
        break;
    case 0x07:
        romSize = 4096;
        break;
    case 0x52:
        romSize = 1152;
        break;
    case 0x53:
        romSize = 1280;
        break;
    case 0x54:
        romSize = 1536;
        break;
    default:
        log("unknown ROMSize %d", _header.ROMSize);
        break;
    }
    switch (_header.RAMSize) {
    case 0x00:
        ramSize = 0;
        break;
    case 0x01:
        ramSize = 2;
        break;
    case 0x02:
        ramSize = 8;
        break;
    case 0x03:
        ramSize = 32;
        break;
    default:
        log("unknown RAMSize %d", _header.RAMSize);
        break;
    }
}
