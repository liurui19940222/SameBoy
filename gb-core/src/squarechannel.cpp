#include "../include/squarechannel.h"

#include <stdio.h>

SquareChannel::SquareChannel() : Channel()
{
    _regs[2] = 0b1000;
}

void SquareChannel::Write(uint16 regIndex, uint8 data)
{
	Channel::Write(regIndex, data);
    switch (regIndex) {
        case 1:
            _lengthCountdown = 64 - (getLengthLoad() & 0x3F);
            break;
    }
}

void SquareChannel::Step()
{
    if (--_timer <= 0) {
        _timer = (2048 - getTimer()) * 4;
        _indexOnDuty = (_indexOnDuty + 1) & 0x7;
    }

    if (IsRunning() && isDACEnabled() && _isDutyPositive()) {
        _outputVolume = _curVolume;
    } else {
        _outputVolume = 0;
    }
}

void SquareChannel::TickSweep()
{
    if (--_curSweepPeriod <= 0) {
        _curSweepPeriod = _getSweepPeriod();
        if (_curSweepPeriod == 0) {
            _curSweepPeriod = 8;
        }
        if (_isSweepEnabled && _getSweepPeriod() > 0) {
            uint16 newFreq = _sweepCalculation();
            if (newFreq <= 2047 && _getSweepShift() > 0) {
                _sweepMask = newFreq;
                setTimerLoad(newFreq);
                _sweepCalculation();
            }
            _sweepCalculation();
        }
    }
}

uint16 SquareChannel::_sweepCalculation()
{
    uint16 newFreq = 0;
    newFreq = _sweepMask >> _getSweepShift();
    if (_isSweepNegate()) {
        newFreq = _sweepMask - newFreq;
    } else {
        newFreq = _sweepMask + newFreq;
    }
    return newFreq;
}

void SquareChannel::onTrigger()
{
    if (_lengthCountdown == 0) {
        _lengthCountdown = 64;
    }
    _timer = (2048 - getTimer()) * 4;
    _isEnvRunning = true;
    _curEnvPeriod = getEnvPeriod();
    _curVolume = getVolume();
    _sweepMask = getTimer();
    _curSweepPeriod = _getSweepPeriod();
    if (_curSweepPeriod == 0) {
        _curSweepPeriod = 8;
    }
    _isSweepEnabled = _curSweepPeriod > 0 || _getSweepShift() > 0;
    if (_getSweepShift() > 0) {
        _sweepCalculation();
    }
}
