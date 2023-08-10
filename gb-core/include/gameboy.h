#pragma once

#include <chrono>
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "disassembler.h"
#include "joypad.h"
#include "cartridge.h"
#include "timer.h"

using namespace std::chrono;

class Gameboy
{
public:
	~Gameboy();

	bool Setup(std::function<void(const uint32*)> draw, std::function<void(const float*, int)> play, const char* romPath, const char* cfgPath);

	void Tick();

	void JoypadPress(Buttons button);

	void JoypadRelease(Buttons button);

	inline CPUStatus GetStatus() const { return _cpu->GetStatus(); }

	inline void Decode(vector<InstructionDetail>& list) { _disassembler->Decode(list); }

private:
	CPU* _cpu;

	PPU* _ppu;

	APU* _apu;

	Timer* _timer;

	MemoryBus* _bus;

	Joypad* _joypad;

	Cartridge* _cartridge;

	InstructionConfigTable* _insCfgTable;

	Disassembler* _disassembler;
};

