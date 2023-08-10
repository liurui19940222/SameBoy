#include <cstring>
#include "../include/framebuffer.h"

GBFramebuffer::GBFramebuffer() {
	ClearBuffer();
}

void GBFramebuffer::SetPixel(int x, int y, uint32 color)
{
	_buffer[x + y * GB_SCREEN_W] = color;
}

uint32 GBFramebuffer::GetPixel(int x, int y) const
{
	return _buffer[x + y * GB_SCREEN_W];
}

const uint32* GBFramebuffer::GetPixels() {
	return _buffer;
}

void GBFramebuffer::ClearBuffer() {
	memset(_buffer, 0xFF, GB_SCREEN_W * GB_SCREEN_H * sizeof(int));
}
