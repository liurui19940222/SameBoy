#include "../include/noisechannel.h"

void NoiseChannel::Write(uint16 regIndex, uint8 data)
{
    switch (regIndex) {
        case 1:
            Channel::Write(regIndex, data & 0x3F);
            _lengthCountdown = 64 - getLengthLoad();
            break;
        default:
            Channel::Write(regIndex, data);
            break;
    }
} 

void NoiseChannel::Step()
{
    if (--_timer <= 0) {
        _timer = _divide[getDivisorCode()] << getClockShift();
        uint8 result = (_lfsr & 0x1) ^ ((_lfsr >> 1) & 0x1);
        _lfsr >>= 1;
        _lfsr |= result << 14;
        if (isWidthMode()) {
            _lfsr &= ~0x40;
            _lfsr |= result << 6;
        }
        if (IsRunning() && isDACEnabled() && (_lfsr & 0x1) == 0) {
            _outputVolume = _curVolume;
        } else {
            _outputVolume = 0;
        }
    }
}

void NoiseChannel::onTrigger()
{
    if (_lengthCountdown == 0) {
        _lengthCountdown = 64;
    }
    _timer = _divide[getDivisorCode()] << getClockShift();
    _curEnvPeriod = getEnvPeriod();
    _isEnvRunning = true;
    _curVolume = getVolume();
    _lfsr = 0x7FFF;
}
