#include "../include/gameboy.h"
#include <thread>

Gameboy::~Gameboy()
{
	if (_insCfgTable) delete _insCfgTable;
	if (_bus) delete _bus;
	if (_disassembler) delete _disassembler;
	if (_cpu) delete _cpu;
	if (_ppu) delete _ppu;
	if (_apu) delete _apu;
	if (_joypad) delete _joypad;
	if (_cartridge) delete _cartridge;
	if (_timer) delete _timer;
}

bool Gameboy::Setup(std::function<void(const uint32*)> draw, std::function<void(const float*, int)> play, const char* romPath, const char* cfgPath) {
	_bus = new MemoryBus();
	_insCfgTable = new InstructionConfigTable();
	if (!_insCfgTable->InitTable(cfgPath)) {
		return false;
	}
	_cartridge = new Cartridge();
	if (!_cartridge->Open(romPath)) {
		return false;
	}
	_disassembler = new Disassembler(_bus, _insCfgTable);
	_cpu = new CPU(_bus, _insCfgTable);
	_ppu = new PPU(_bus, draw);
	_apu = new APU(_bus, play);
	_joypad = new Joypad(_bus);
	_timer = new Timer(_bus);
	_bus->SetIODevice(IOType::CPU, _cpu);
	_bus->SetIODevice(IOType::PPU, _ppu);
	_bus->SetIODevice(IOType::APU, _apu);
	_bus->SetIODevice(IOType::Timer, _timer);
	_bus->SetIODevice(IOType::Joypad, _joypad);
	_bus->SetIODevice(IOType::Cartridge, _cartridge);
	return true;
}

void Gameboy::Tick()
{
	auto begin = high_resolution_clock::now();

	uint8 cycles = _cpu->Step();
	_timer->Step(cycles);
	_ppu->Step(cycles);
	_apu->Step(cycles);

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(end - begin);
	while (duration.count() < cycles * 200) {
		end = high_resolution_clock::now();
		duration = duration_cast<nanoseconds>(end - begin);
	}
}

void Gameboy::JoypadPress(Buttons button) {
	_joypad->Press(button);
}

void Gameboy::JoypadRelease(Buttons button) {
	_joypad->Release(button);
}