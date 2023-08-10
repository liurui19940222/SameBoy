#pragma once

#include "datatype.h"

class GBFramebuffer
{
public:
	GBFramebuffer();

	void SetPixel(int x, int y, uint32 color);

	uint32 GetPixel(int x, int y) const;

	const uint32* GetPixels();

	void ClearBuffer();

private:
	uint32 _buffer[GB_SCREEN_W * GB_SCREEN_H];
};

