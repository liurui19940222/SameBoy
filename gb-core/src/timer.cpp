#include "../include/timer.h"
#include "../include/tools.h"

Timer::Timer(MemoryBus* bus)
{
	_bus = bus;
	_tima = _tma = _tac = 0;
	_divCounter = 0;
}

void Timer::Write(uint16 address, uint8 value)
{
	switch (address)
	{
	case MEM_DIV:
		_divCounter = 0;
		break;
	case MEM_TIMA:
		_tima = value;
		break;
	case MEM_TMA:
		_tma = value;
		break;
	case MEM_TAC:
		_tac = value;
		break;
	}
}

uint8 Timer::Read(uint16 address)
{
	switch (address)
	{
	case MEM_DIV:
		return (uint8)(_divCounter >> 8);
	case MEM_TIMA:
		return _tima;
	case MEM_TMA:
		return _tma;
	case MEM_TAC:
		return _tac;
	}
	return 0xFF;
}

void Timer::Step(uint8 cycles)
{
	uint16 oldDiv = _divCounter;
	_divCounter += cycles;

	if (checkBit(_tac, 2)) {
		int selectBit = 0;
		switch (_tac & 3)
		{
		case 0: selectBit = 0x200; break;
		case 1: selectBit = 0x008; break;
		case 2: selectBit = 0x020; break;
		case 3: selectBit = 0x080; break;
		}
		if ((oldDiv & selectBit) != 0 && (_divCounter & selectBit) == 0) {
			if (_tima == 0xFF) {
				_tima = _tma;
				_bus->SetBit(MEM_IF, (uint8)InterFlags::Timer, 1);
			}
			else {
				_tima++;
			}
		}
	}
}
