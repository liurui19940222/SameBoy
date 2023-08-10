#include "channel.h"
#include "datatype.h"

#pragma once
class NoiseChannel : public Channel
{
public:
	void Write(uint16 regIndex, uint8 data) override;
	void Step() override;

protected:
	void onTrigger() override;

private:
	const uint32 _divide[8] = { 8, 16, 32, 48, 64, 80, 96, 112 };
	uint16 _lfsr = 0;

	inline bool isWidthMode() const { return (_regs[3] & 0x8) == 0x8; }
	inline uint8 getDivisorCode() const { return _regs[3] & 0x7; } 
	inline uint8 getClockShift() const { return (_regs[3] >> 4) & 0xF; }
};

