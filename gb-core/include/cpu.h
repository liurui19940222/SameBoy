#pragma once

#include "datatype.h"
#include "memory.h"
#include "instruction.h"

struct Registers {
	struct {
		union {
			struct
			{
				uint8 F;
				uint8 A;
				uint8 C;
				uint8 B;
				uint8 E;
				uint8 D;
				uint8 L;
				uint8 H;
			} s;
			struct
			{
				uint16 AF;
				uint16 BC;
				uint16 DE;
				uint16 HL;
			} d;
		};
	};
	uint16 PC;
	uint16 SP;
};

enum class RegFlags : uint8 {
	Z = 7,
	N = 6,
	H = 5,
	C = 4
};

struct CPUStatus {
	Registers Reg;
	uint8 IME;
	uint8 IE;
	uint8 IF;
	uint64 TICK;
	uint8 DIV, TIMA, TMA, TAC;
};

class CPU : public IODevice
{
public:
	CPU(MemoryBus* bus, InstructionConfigTable* insCfgTable);

	virtual ~CPU() {}

	uint8 Step();

	CPUStatus GetStatus() const;

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

private:
	Registers _regs;

	MemoryBus* _bus;

	InstructionConfigTable* _insCfgTable;

	uint8 _ime, _ie, _if;

	uint8 _enableIMEDelay;

	bool _halted;

	uint64 _tic;
	
	void _setFlag(RegFlags flag, uint8 bitValue);

	int _checkFlag(RegFlags flag);

	int _increase(int value);

	int _decrease(int value);

	uint8 _add(uint8 x, uint8 y);

	uint8 _adc(uint8 x, uint8 y);

	uint16 _add16(uint16 x, uint16 y);

	uint8 _sub(uint8 x, uint8 y);

	uint8 _sbc(uint8 x, uint8 y);

	void _push(uint8 x);

	void _push16(uint16 x);

	uint8 _pop();

	uint16 _pop16();

	uint8 _and(uint8 x, uint8 y);

	uint8 _or(uint8 x, uint8 y);

	uint8 _xor(uint8 x, uint8 y);

	uint8 _swap(uint8 x);

	uint8 _daa(uint8 x);

	uint8 _cpl(uint8 x);

	void _ccf();

	void _scf();

	uint8 _rlc(uint8 x);

	uint8 _rlca(uint8 x);

	uint8 _rl(uint8 x);

	uint8 _rla(uint8 x);

	uint8 _rrc(uint8 x);

	uint8 _rrca(uint8 x);

	uint8 _rr(uint8 x);

	uint8 _rra(uint8 x);

	uint8 _sla(uint8 x);

	uint8 _sra(uint8 x);

	uint8 _srl(uint8 x);

	void _bit(uint8 x, uint8 bitIndex);

	uint8 _set(uint8 x, uint8 bitIndex);

	uint8 _res(uint8 x, uint8 bitIndex);

	void _add_sp_n(int8 value);

	bool _jp(RegFlags flag, uint8 flagValue, uint16 address);

	bool _jr(RegFlags flag, uint8 flagValue, uint8 n);

	bool _call(RegFlags flag, uint8 flagValue, uint16 address);

	void _rst(uint16 nextPC, uint16 jpAddress);

	bool _ret(RegFlags flag, uint8 flagValue);

	bool _fetch(uint16* opCode, InstructionConfig* config);

	uint8 _execute(uint16 opCode, InstructionConfig* config);

	void _jumpToVector(uint16 address);

	void _checkInterrupts();

	void _printStatus(uint16 address, uint16 opCode, InstructionConfig& ins);
};

