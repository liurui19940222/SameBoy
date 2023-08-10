#include "../include/channel.h"

Channel::Channel() { _timer = 0; }

Channel::~Channel() {}

uint8 Channel::Read(uint16 regIndex) { return _regs[regIndex]; }

void Channel::Write(uint16 regIndex, uint8 data)
{
    _regs[regIndex] = data;
    switch (regIndex) {
        case 2:
            _curEnvPeriod = getEnvPeriod();
            _curVolume = getVolume();
            break;
        case 4:
            if ((data & 0x80) == 0x80) {
                onTrigger();
            }
            break;
    }
}

void Channel::Step() {}

void Channel::TickLength()
{
    if (_lengthCountdown > 0 && isLengthEnabled()) {
        _lengthCountdown--;
    }
}

void Channel::TickEnv()
{
    if (--_curEnvPeriod <= 0) {
        _curEnvPeriod = getEnvPeriod();
        if (_curEnvPeriod == 0) {
            _curEnvPeriod = 8;
        }
        if (_isEnvRunning && getEnvPeriod() > 0) {
            if (isEnvAddMode() && _curVolume < 15) {
                _curVolume++;
            } else if (!isEnvAddMode() && _curVolume > 0) {
                _curVolume--;
            }
        }
        if (_curVolume == 0 || _curVolume == 15) {
            _isEnvRunning = false;
        }
    }
}

uint16 Channel::getTimer() const
{
    return ((_regs[4] & 0x7) << 8) | _regs[3];
}

void Channel::setTimerLoad(uint16 value)
{
    _regs[3] = value & 0xFF;
    _regs[4] = ((_regs[4] >> 3) << 3) | ((value >> 8) & 0x7);
}

bool Channel::isDACEnabled() const { return (_regs[2] & 0xF8) != 0; }

uint8 Channel::getLengthLoad() const { return _regs[1] & 0x3F; }