#include "../include/ppu.h"
#include <vector>

uint32 PPU::color[4] = { 0xFFFFFFFF, 0xFFC0C0C0, 0xFF606060, 0xFF000000 };

PPU::PPU(MemoryBus* bus, std::function<void(const uint32*)> draw) {
	_bus = bus;
	_draw = draw;

	_lcd_y = _lcd_yc = _lcd_stat = 0;
	_lcdc = 0x91;

	framebuffer = new GBFramebuffer();
}

void PPU::Step(uint8 cycles) {
	_cyclesCounter += cycles;
	switch ((PPUMode)(_lcd_stat & 0b11)) {
	case PPUMode::HBlank:
		if (_cyclesCounter >= DOTS_HBLANK) {
			_cyclesCounter -= DOTS_HBLANK;
			_scanline(_lcd_y);
			if (++_lcd_y == 144) {
				SetMode(PPUMode::VBlank);
				_bus->SetBit(MEM_IF, (uint8)InterFlags::VBlank, 1);
			}
			else {
				SetMode(PPUMode::SearchingOAM);
			}
		}
		break;
	case PPUMode::VBlank:
		if (_cyclesCounter >= DOTS_VBLANK) {
			_cyclesCounter -= DOTS_VBLANK;
			if (++_lcd_y == 154) {
				_lcd_y = 0;
				_postRender();
				framebuffer->ClearBuffer();
				SetMode(PPUMode::SearchingOAM);
			};
		}
		break;
	case PPUMode::SearchingOAM:
		if (_cyclesCounter >= DOTS_OAM) {
			_cyclesCounter -= DOTS_OAM;
			SetMode(PPUMode::TransferData);
		}
		break;
	case PPUMode::TransferData:
		if (_cyclesCounter >= DOTS_DRAW) {
			_cyclesCounter -= DOTS_DRAW;

			if (checkBit(_lcd_stat, 3)) {
				_bus->SetBit(MEM_IF, (uint8)InterFlags::LCDSTAT, 1);
			}

			if (checkBit(_lcd_stat, 6) && _lcd_y == _lcd_yc) {
				_bus->SetBit(MEM_IF, (uint8)InterFlags::LCDSTAT, 1);
			}

			setBit(_lcd_stat, 2, _lcd_y == _lcd_yc ? 1 : 0);
			SetMode(PPUMode::HBlank);
		}
		break;
	}
}

void PPU::Write(uint16 address, uint8 value) {
	switch (address) {
	case MEM_LY:
		_lcd_y = value;
		break;
	case MEM_LYC:
		_lcd_yc = value;
		break;
	case MEM_LCD_STAT:
		_lcd_stat = value;
		break;
	case MEM_SCX:
		_scrollx = value;
		break;
	case MEM_SCY:
		_scrolly = value;
		break;
	case MEM_WNDX:
		_wndx = value;
		break;
	case MEM_WNDY:
		_wndy = value;
		break;
	case MEM_LCDC:
		_lcdc = value;
		break;
	case MEM_DMA:
		_dmaTransfter(value);
		break;
	case MEM_BGP:
		_bg_palette = value;
		break;
	case MEM_OBJP0:
		_obj_palette0 = value;
		break;
	case MEM_OBJP1:
		_obj_palette1 = value;
		break;
	}
}

uint8 PPU::Read(uint16 address) {
	switch (address) {
	case MEM_LCDC:
		return _lcdc;
	case MEM_LY:
		return _lcd_y;
	case MEM_LYC:
		return _lcd_yc;
	case MEM_LCD_STAT:
		return _lcd_stat;
	case MEM_SCX:
		return _scrollx;
	case MEM_SCY:
		return _scrolly;
	case MEM_WNDX:
		return _wndx;
	case MEM_WNDY:
		return _wndy;
	case MEM_DMA:
		return 0xFF;
	case MEM_BGP:
		return _bg_palette;
	case MEM_OBJP0:
		return _obj_palette0;
	case MEM_OBJP1:
		return _obj_palette1;
	default:
		return 0xFF;
	}
}

void PPU::_postRender() {
	_draw(framebuffer->GetPixels());
}

void PPU::_scanline(uint8 line) {
	if (!_checkLCDC(LCDCFlags::LCDandPPUEnable)) {
		return;
	}
	_drawBg(line);
	_drawWnd(line);
	_drawSprites(line);
}

void PPU::_drawBg(uint8 line) {
	if (!_checkLCDC(LCDCFlags::BGandWndEnable)) {
		return;
	}
	uint16 mapArea = _checkLCDC(LCDCFlags::BGTileMapArea) ? 0x9C00 : 0x9800;
	uint16 tileSet = _checkLCDC(LCDCFlags::BGandWndTileDataArea) ? 0x8000 : 0x9000;
	uint8 tileMapX, tileMapY, tileID, tileX, tileY, lowByte, highByte, pixel;
	int byteAddress;
	int tileIndex;
	for (uint8 x = 0; x < GB_SCREEN_W; ++x) {
		tileMapX = ((x + _scrollx) & 0xFF) / 8;
		tileMapY = ((line + _scrolly) & 0xFF) / 8;
		tileID = _bus->ReadByte(mapArea + tileMapY * 32 + tileMapX);
		tileX = (x + _scrollx) % 8;
		tileY = (line + _scrolly) % 8;
		if (_checkLCDC(LCDCFlags::BGandWndTileDataArea)) {
			tileIndex = tileID;
		}
		else {
			tileIndex = tileID > 127 ? (tileID - 256) : tileID;
		}
		byteAddress = tileSet + tileIndex * 16 + tileY * 2;
		lowByte = _bus->ReadByte((uint16)byteAddress);
		highByte = _bus->ReadByte((uint16)byteAddress + 1);
		pixel = ((lowByte >> (7 - tileX)) & 0x1) | (((highByte >> (7 - tileX)) & 0x1) << 1);
		framebuffer->SetPixel(x, line, _getRealColor(_bg_palette, pixel));
	}
}

void PPU::_drawWnd(uint8 line) {
	if (!_checkLCDC(LCDCFlags::BGandWndEnable) || !_checkLCDC(LCDCFlags::WndEnable) || line < _wndy) {
		return;
	}
	uint16 mapArea = _checkLCDC(LCDCFlags::WndTileMapArea) ? 0x9C00 : 0x9800;
	uint16 tileSet = _checkLCDC(LCDCFlags::BGandWndTileDataArea) ? 0x8000 : 0x9000;
	uint8 tileMapX, tileMapY, tileID, tileX, tileY, lowByte, highByte, pixel;
	int byteAddress;
	int tileIndex;
	uint8 left = _wndx - 7;
	for (uint8 x = left; x < GB_SCREEN_W; ++x) {
		tileMapX = (x + 7 - _wndx) / 8;
		tileMapY = (line - _wndy) / 8;
		tileID = _bus->ReadByte(mapArea + tileMapY * 32 + tileMapX);
		tileX = (x + 7 - _wndx) % 8;
		tileY = (line - _wndy) % 8;
		if (_checkLCDC(LCDCFlags::BGandWndTileDataArea)) {
			tileIndex = tileID;
		}
		else {
			tileIndex = tileID > 127 ? (tileID - 256) : tileID;
		}
		byteAddress = tileSet + tileIndex * 16 + tileY * 2;
		lowByte = _bus->ReadByte((uint16)byteAddress);
		highByte = _bus->ReadByte((uint16)byteAddress + 1);
		pixel = ((lowByte >> (7 - tileX)) & 0x1) | (((highByte >> (7 - tileX)) & 0x1) << 1);
		framebuffer->SetPixel(x, line, _getRealColor(_bg_palette, pixel));
	}
}

void PPU::_drawSprites(uint8 line) {
	if (!_checkLCDC(LCDCFlags::ObjEnable)) {
		return;
	}
	std::vector<Tile> visibleSprites;
	uint8 y, tileHeight = _checkLCDC(LCDCFlags::ObjSize) ? 16 : 8;;
	for (uint16 address = MEM_OAM_BEGIN; address < MEM_OAM_END && visibleSprites.size() < 10; address += 4) {
		y = _bus->ReadByte(address);
		if (y == 0 || y >= 160) {
			continue;
		}
		if (line >= y - 16 && line < y - 16 + tileHeight) {
			visibleSprites.push_back(Tile{ y, _bus->ReadByte(address + 1),
				_bus->ReadByte(address + 2), _bus->ReadByte(address + 3) });
		}
	}
	if (visibleSprites.size() == 0) {
		return;
	}
	uint8 tileX, tileY, lowByte, highByte, pixel, palette;
	int realColor;
	for (uint8 x = 0; x < GB_SCREEN_W; ++x) {
		for (auto it = visibleSprites.begin(); it != visibleSprites.end(); ++it) {
			if (it->x == 0 || it->x >= GB_SCREEN_W) {
				continue;
			}
			if (x >= it->x - 8 && x < it->x) {
				// behind bg
				if (checkBit(it->attribute, 7) && framebuffer->GetPixel(x, line) != color[0]) {
					continue;
				}
				// x flip
				if (checkBit(it->attribute, 5)) {
					tileX = 7 - (x + 8 - it->x);
				}
				else {
					tileX = x + 8 - it->x;
				}
				// y flip
				if (checkBit(it->attribute, 6)) {
					tileY = tileHeight - 1 - (line + 16 - it->y);
				}
				else {
					tileY = line + 16 - it->y;
				}
				lowByte = _bus->ReadByte(0x8000 + it->index * 16 + tileY * 2);
				highByte = _bus->ReadByte(0x8000 + it->index * 16 + tileY * 2 + 1);
				pixel = ((lowByte >> (7 - tileX)) & 0x1) | (((highByte >> (7 - tileX)) & 0x1) << 1);
				if (pixel == 0) {
					continue;
				}
				palette = checkBit(it->attribute, 4) ? _obj_palette1 : _obj_palette0;
				realColor = _getRealColor(palette, pixel);
				framebuffer->SetPixel(x, line, realColor);
				break;
			}
		}
	}
}

void PPU::_dmaTransfter(uint8 value) {
	uint16 start_address = value * 0x100;

	for (uint8 i = 0x0; i <= 0x9F; i++) {
		uint16 from_address = start_address + i;
		uint16 to_address = 0xFE00 + i;

		uint8 value_at_address = _bus->ReadByte(from_address);
		_bus->WriteByte(to_address, value_at_address);
	}
}
