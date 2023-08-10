#include "../include/cpu.h"

#include <assert.h>

#include <cstring>
#include <stdexcept>

#include "../include/opcode.h"
#include "../include/tools.h"

CPU::CPU(MemoryBus* bus, InstructionConfigTable* insCfgTable)
{
    memset(&this->_regs, 0, sizeof(Registers));
    this->_bus = bus;
    this->_insCfgTable = insCfgTable;
    _regs.PC = 0x100;
    _regs.s.A = 1;
    _regs.s.C = 0x13;
    _regs.s.E = 0xD8;
    _regs.s.H = 1;
    _regs.s.L = 0x4D;
    _regs.SP = 0xFFFE;
    _setFlag(RegFlags::Z, 1);
    _setFlag(RegFlags::H, 1);
    _setFlag(RegFlags::C, 1);
    _enableIMEDelay = 0xFF;
    _ie = _if = _ime = 0;
    _halted = false;
}

uint8 CPU::Step()
{
    assert(_regs.PC >= 0 && _regs.PC <= 0xFFFF);

    _checkInterrupts();

    _tic++;

    if (_halted) {
        return 4;
    }

    InstructionConfig insCfg;
    uint16 opCode;
    if (!_fetch(&opCode, &insCfg)) {
        string str = format("fetch instruction fail at 0x%x", _regs.PC);
        throw new std::runtime_error(str.c_str());
    }

    //_printStatus(_regs.PC, opCode, insCfg);

    return _execute(opCode, &insCfg);
}

CPUStatus CPU::GetStatus() const
{
    return CPUStatus{_regs,
                     _ime,
                     _bus->ReadByte(MEM_IE),
                     _bus->ReadByte(MEM_IF),
                     _tic,
                     _bus->ReadByte(MEM_DIV),
                     _bus->ReadByte(MEM_TIMA),
                     _bus->ReadByte(MEM_TMA),
                     _bus->ReadByte(MEM_TAC)};
}

void CPU::Write(uint16 address, uint8 value)
{
    switch (address) {
        case MEM_IE:
            _ie = value;
            break;
        case MEM_IF:
            _if = value;
            break;
    }
}

uint8 CPU::Read(uint16 address)
{
    switch (address) {
        case MEM_IE:
            return _ie;
        case MEM_IF:
            return _if;
    }
    return 0xFF;
}

void CPU::_printStatus(uint16 address, uint16 opCode, InstructionConfig& ins)
{
    log("\n%04x\t%04x\t%-10s", address, opCode, ins.mnemonic.c_str());
    log("AF=%04x\tBC=%04x\tDE=%04x\tHL=%04x\tSP=%04x\tPC=%04x", _regs.d.AF,
        _regs.d.BC, _regs.d.DE, _regs.d.HL, _regs.SP, _regs.PC);
    log("Z=%d\tN=%d\tH=%d\tC=%d", _checkFlag(RegFlags::Z),
        _checkFlag(RegFlags::N), _checkFlag(RegFlags::H),
        _checkFlag(RegFlags::C));
    log("IME=%d\tIE=%02x\tIF=%02x\tCOUNT:%d", _ime, _bus->ReadByte(MEM_IE),
        _bus->ReadByte(MEM_IF), _tic);
}

void CPU::_setFlag(RegFlags flag, uint8 bitValue)
{
    if (bitValue != 0) {
        _regs.s.F |= (1 << (uint8)flag);
    } else {
        _regs.s.F &= ~(1 << (uint8)flag);
    }
}

int CPU::_checkFlag(RegFlags flag)
{
    return (_regs.s.F & (1 << (uint8)flag)) == 0 ? 0 : 1;
}

int CPU::_increase(int value)
{
    value++;
    _setFlag(RegFlags::H, (value & 0xf) == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::Z, ((value & 0xff) == 0 ? 1 : 0));
    return value & 0xff;
}

int CPU::_decrease(int value)
{
    value--;
    _setFlag(RegFlags::H, (value & 0xf) == 0xf ? 1 : 0);
    _setFlag(RegFlags::N, 1);
    _setFlag(RegFlags::Z, ((value & 0xff) == 0 ? 1 : 0));
    return value & 0xff;
}

uint8 CPU::_add(uint8 value1, uint8 value2)
{
    int result = value1 + value2;
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H,
             (((value1 & 0xf) + (value2 & 0xf)) & 0x10) != 0 ? 1 : 0);
    _setFlag(RegFlags::C, (result & 0x100) != 0 ? 1 : 0);
    _setFlag(RegFlags::Z, (result & 0xff) == 0 ? 1 : 0);
    return (uint8)(int8)(result & 0xff);
}

uint8 CPU::_adc(uint8 reg, uint8 value)
{
    uint8 carry = _checkFlag(RegFlags::C);

    uint32 result_full = reg + value + carry;
    uint8 result = static_cast<uint8>(result_full);

    _setFlag(RegFlags::Z, result == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, ((reg & 0xf) + (value & 0xf) + carry) > 0xf ? 1 : 0);
    _setFlag(RegFlags::C, result_full > 0xff ? 1 : 0);

    return result;
}

uint16 CPU::_add16(uint16 value1, uint16 value2)
{
    int result = value1 + value2;
    _setFlag(RegFlags::H,
             (((value1 & 0xfff) + (value2 & 0xfff)) & 0x1000) != 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::C, (result & 0x10000) != 0 ? 1 : 0);
    return (uint16)(int16)(result & 0xffff);
}

uint8 CPU::_sub(uint8 value1, uint8 value2)
{
    int result = value1 - value2;
    _setFlag(RegFlags::N, 1);
    _setFlag(RegFlags::H,
             (((value1 & 0xf) - (value2 & 0xf)) & 0x10) != 0 ? 1 : 0);
    _setFlag(RegFlags::C, (result & 0x100) != 0 ? 1 : 0);
    _setFlag(RegFlags::Z, (result & 0xff) == 0 ? 1 : 0);
    return (uint8)(int8)(result & 0xff);
}

uint8 CPU::_sbc(uint8 reg, uint8 value)
{
    uint8 carry = _checkFlag(RegFlags::C);

    int result_full = reg - value - carry;
    uint8 result = static_cast<uint8>(result_full);

    _setFlag(RegFlags::Z, result == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 1);
    _setFlag(RegFlags::H, ((reg & 0xf) - (value & 0xf) - carry) < 0 ? 1 : 0);
    _setFlag(RegFlags::C, result_full < 0 ? 1 : 0);

    return result;
}

void CPU::_push(uint8 x)
{
    _regs.SP -= 1;
    _bus->WriteByte(_regs.SP, x);
}

void CPU::_push16(uint16 x)
{
    _regs.SP -= 2;
    _bus->Write2Byte(_regs.SP, x);
}

uint8 CPU::_pop()
{
    uint8 result = _bus->ReadByte(_regs.SP);
    _regs.SP += 1;
    return result;
}

uint16 CPU::_pop16()
{
    uint16 result = _bus->Read2Byte(_regs.SP);
    _regs.SP += 2;
    return result;
}

uint8 CPU::_and(uint8 x, uint8 y)
{
    uint8 result = x & y;
    _setFlag(RegFlags::Z, result == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 1);
    _setFlag(RegFlags::C, 0);
    return result;
}

uint8 CPU::_or(uint8 x, uint8 y)
{
    uint8 result = x | y;
    _setFlag(RegFlags::Z, result == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, 0);
    return result;
}

uint8 CPU::_xor(uint8 x, uint8 y)
{
    uint8 result = x ^ y;
    _setFlag(RegFlags::Z, result == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, 0);
    return result;
}

uint8 CPU::_swap(uint8 x)
{
    uint8 high = x >> 4;
    uint8 low = x & 0xF;
    uint8 result = (low << 4) | high;
    _setFlag(RegFlags::Z, result == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, 0);
    return result;
}

uint8 CPU::_daa(uint8 x)
{
    if (!_checkFlag(RegFlags::N)) {
        if (_checkFlag(RegFlags::C) || x > 0x99) {
            x += 0x60;
            _setFlag(RegFlags::C, 1);
        }
        if (_checkFlag(RegFlags::H) || (x & 0x0f) > 0x09) {
            x += 0x6;
        }
    } else {
        if (_checkFlag(RegFlags::C)) {
            x -= 0x60;
        }
        if (_checkFlag(RegFlags::H)) {
            x -= 0x6;
        }
    }
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::H, 0);
    return x;
}

uint8 CPU::_cpl(uint8 x)
{
    _setFlag(RegFlags::N, 1);
    _setFlag(RegFlags::H, 1);
    return ~x;
}

void CPU::_ccf()
{
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    if (_checkFlag(RegFlags::C)) {
        _setFlag(RegFlags::C, 0);
    } else {
        _setFlag(RegFlags::C, 1);
    }
}

void CPU::_scf()
{
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, 1);
}

uint8 CPU::_rlc(uint8 x)
{
    uint8 carray = (x & 0x80) != 0 ? 1 : 0;
    _setFlag(RegFlags::C, carray);
    x = (x << 1) | carray;
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    return x;
}

uint8 CPU::_rlca(uint8 x)
{
    uint8 value = _rlc(x);
    _setFlag(RegFlags::Z, 0);
    return value;
}

uint8 CPU::_rl(uint8 x)
{
    uint8 carry = _checkFlag(RegFlags::C);
    uint8 carry_flag = (x & 0x80) != 0 ? 1 : 0;
    x = (x << 1) | carry;
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, carry_flag);
    return x;
}

uint8 CPU::_rla(uint8 x)
{
    uint8 value = _rl(x);
    _setFlag(RegFlags::Z, 0);
    return value;
}

uint8 CPU::_rrc(uint8 x)
{
    uint8 carry_flag = (x & 0x01) != 0 ? 1 : 0;
    x = (x >> 1) | (carry_flag << 7);
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, carry_flag);
    return x;
}

uint8 CPU::_rrca(uint8 x)
{
    uint8 value = _rrc(x);
    _setFlag(RegFlags::Z, 0);
    return value;
}

uint8 CPU::_rr(uint8 x)
{
    uint8_t carry = _checkFlag(RegFlags::C);
    uint8 carry_flag = (x & 0x01) != 0 ? 1 : 0;
    x = (x >> 1) | (carry << 7);
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, carry_flag);
    return x;
}

uint8 CPU::_rra(uint8 x)
{
    uint8 value = _rr(x);
    _setFlag(RegFlags::Z, 0);
    return value;
}

uint8 CPU::_sla(uint8 x)
{
    uint8 carry_flag = (x & 0x80) != 0 ? 1 : 0;
    x <<= 1;
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, carry_flag);
    return x;
}

uint8 CPU::_sra(uint8 x)
{
    uint8 carry_flag = (x & 0x01) != 0 ? 1 : 0;
    x = (x & 0x80) | (x >> 1);
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, carry_flag);
    return x;
}

uint8 CPU::_srl(uint8 x)
{
    uint8 carry_flag = (x & 0x01) != 0 ? 1 : 0;
    x >>= 1;
    _setFlag(RegFlags::Z, x == 0 ? 1 : 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 0);
    _setFlag(RegFlags::C, carry_flag);
    return x;
}

void CPU::_bit(uint8 x, uint8 bitIndex)
{
    uint8 flag = (x & (1 << bitIndex)) == 0 ? 1 : 0;
    _setFlag(RegFlags::Z, flag);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H, 1);
}

uint8 CPU::_set(uint8 x, uint8 bitIndex) { return x | (1 << bitIndex); }

uint8 CPU::_res(uint8 x, uint8 bitIndex) { return x & ~(1 << bitIndex); }

void CPU::_add_sp_n(int8 value)
{
    int imm = value;
    int result = (int)_regs.SP + imm;
    _setFlag(RegFlags::Z, 0);
    _setFlag(RegFlags::N, 0);
    _setFlag(RegFlags::H,
             (((_regs.SP ^ imm ^ (result & 0xFFFF)) & 0x10) == 0x10));
    _setFlag(RegFlags::C,
             ((_regs.SP ^ imm ^ (result & 0xFFFF)) & 0x100) == 0x100);
    _regs.SP = (uint16)result;
}

bool CPU::_jp(RegFlags flag, uint8 flagValue, uint16 address)
{
    if (_checkFlag(flag) == flagValue) {
        _regs.PC = address;
        return true;
    }
    return false;
}

bool CPU::_jr(RegFlags flag, uint8 flagValue, uint8 n)
{
    if (_checkFlag(flag) == flagValue) {
        _regs.PC += (int8)n;
        return true;
    }
    return false;
}

bool CPU::_call(RegFlags flag, uint8 flagValue, uint16 address)
{
    if (_checkFlag(flag) == flagValue) {
        _push16(_regs.PC);
        _regs.PC = address;
        return true;
    }
    return false;
}

void CPU::_rst(uint16 nextPC, uint16 jpAddress)
{
    _push16(nextPC);
    _regs.PC = jpAddress;
}

bool CPU::_ret(RegFlags flag, uint8 flagValue)
{
    if (_checkFlag(flag) == flagValue) {
        _regs.PC = _pop16();
        return true;
    }
    return false;
}

bool CPU::_fetch(uint16* opCode, InstructionConfig* config)
{
    bool rightIns;
    uint8 firstCode, secondCode;
    firstCode = _bus->ReadByte(_regs.PC);

    if (firstCode == 0xCB) {
        secondCode = _bus->ReadByte(_regs.PC + 1);
        rightIns = _insCfgTable->GetInstruction(secondCode, true, config);
        *opCode = 0xCB00 | secondCode;
    } else {
        rightIns = _insCfgTable->GetInstruction(firstCode, false, config);
        *opCode = firstCode;
    }
    if (!rightIns) {
        return false;
    }

    return rightIns;
}

void CPU::_jumpToVector(uint16 address)
{
    _push16(_regs.PC);
    _regs.PC = address;
    _halted = false;
    _ime = 0;
}

void CPU::_checkInterrupts()
{
    if (_enableIMEDelay != 0xFF) {
        if (--_enableIMEDelay == 0) {
            _enableIMEDelay = 0xFF;
            _ime = 1;
        }
    }

    if (_ime == 0) {
        return;
    }

    uint8 interrupts = _if & _ie;

    if (interrupts == 0) {
        return;
    }

    if (checkBit(interrupts, (uint8)InterFlags::VBlank)) {
        setBit(_if, (uint8)InterFlags::VBlank, 0);
        _jumpToVector(0x40);
    } else if (checkBit(interrupts, (uint8)InterFlags::LCDSTAT)) {
        setBit(_if, (uint8)InterFlags::LCDSTAT, 0);
        _jumpToVector(0x48);
    } else if (checkBit(interrupts, (uint8)InterFlags::Timer)) {
        setBit(_if, (uint8)InterFlags::Timer, 0);
        _jumpToVector(0x50);
    } else if (checkBit(interrupts, (uint8)InterFlags::Serial)) {
        setBit(_if, (uint8)InterFlags::Serial, 0);
        _jumpToVector(0x58);
    } else if (checkBit(interrupts, (uint8)InterFlags::Joypad)) {
        setBit(_if, (uint8)InterFlags::Joypad, 0);
        _jumpToVector(0x60);
    }
}
