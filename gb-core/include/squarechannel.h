#include "channel.h"
#include "datatype.h"

#pragma once
class SquareChannel : public Channel {
public:
    SquareChannel();
    void Step() override;
    void Write(uint16 regIndex, uint8 data) override;
    void TickSweep();

protected:
    void onTrigger() override;

private:
    const uint8 dutyTable[8] = { 0b1, 0b10000001, 0b10000111, 0b1111110 };
    int _indexOnDuty = 0;
    int _curSweepPeriod = 0;
    uint16 _sweepMask = 0;
    bool _isSweepEnabled = false;

    uint16 _sweepCalculation();
    inline bool _isSweepNegate() const { return (_regs[0] & 0x8) == 0x8; }
    inline uint8 _getSweepPeriod() const { return (_regs[0] >> 4) & 0x7; }
    inline uint8 _getSweepShift() const { return _regs[0] & 0x7; }
    inline uint32 _getDuty() const { return (_regs[1] >> 6) & 0x3; }
    inline bool _isDutyPositive() const { return dutyTable[_getDuty()] & (1 << _indexOnDuty); }
};
