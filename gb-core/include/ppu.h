#pragma once

#include "memory.h"
#include "framebuffer.h"
#include "tools.h"
#include <functional>

enum class PPUMode {
	HBlank = 0,
	VBlank = 1,
	SearchingOAM = 2,
	TransferData = 3
};

enum class LCDCFlags {
	BGandWndEnable,			// BG and Window enable/priority	0=Off, 1=On
	ObjEnable,				// OBJ enable	0=Off, 1=On
	ObjSize,				// OBJ size	0=8��8, 1=8��16
	BGTileMapArea,			// BG tile map area	0=9800-9BFF, 1=9C00-9FFF
	BGandWndTileDataArea,	// BG and Window tile data area	0=8800-97FF, 1=8000-8FFF
	WndEnable,				// Window enable	0=Off, 1=On
	WndTileMapArea,			// Window tile map area	0=9800-9BFF, 1=9C00-9FFF
	LCDandPPUEnable			// LCD and PPU enable	0=Off, 1=On
};

struct Tile {
	uint8 y, x;
	uint8 index;
	uint8 attribute;
};

// the sum of dots must to be 70224 per frame
const int DOTS_OAM = 80;
const int DOTS_DRAW = 172;
const int DOTS_HBLANK = 204;
const int DOTS_VBLANK = DOTS_OAM + DOTS_DRAW + DOTS_HBLANK;

class PPU : public IODevice
{
public:
	PPU(MemoryBus* bus, std::function<void(const uint32*)> draw);

	virtual ~PPU() {}

	void Step(uint8 cycles);

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

private:
	static uint32 color[4];

	std::function<void(const uint32*)> _draw;

	MemoryBus* _bus;

	GBFramebuffer* framebuffer;

	uint8 _bg_palette, _obj_palette0, _obj_palette1;

	uint8 _lcd_y, _lcd_yc, _lcd_stat, _lcdc;

	uint8 _scrollx, _scrolly, _wndx, _wndy;

	int _cyclesCounter;

	void _postRender();

	void _scanline(uint8 line);

	void _drawBg(uint8 line);

	void _drawWnd(uint8 line);

	void _drawSprites(uint8 line);

	void _dmaTransfter(uint8 value);

	inline void SetMode(PPUMode mode) { _lcd_stat = (_lcd_stat >> 2) << 2 | (uint8)mode; }

	inline bool _checkLCDC(LCDCFlags flag) { return checkBit(_lcdc, (uint8)flag); }

	inline int _getRealColor(uint8 palette, uint8 index) { return color[(palette >> (index * 2)) & 3]; }
};

