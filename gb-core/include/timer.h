#pragma once

#include "memory.h"

class Timer : public IODevice
{
public:
	Timer(MemoryBus* bus);

	virtual ~Timer(){}

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

	void Step(uint8 cycles);

private:
	MemoryBus* _bus;

	uint8 _tima, _tma, _tac;

	uint16 _divCounter;
};

