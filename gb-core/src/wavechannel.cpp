#include "../include/wavechannel.h"

WaveChannel::WaveChannel() : Channel() {
    _waveData.resize(16);
}

void WaveChannel::Write(uint16 regIndex, uint8 data)
{
    Channel::Write(regIndex, data);

    switch (regIndex) {
        case 1:
            _lengthCountdown = 256 - getLengthLoad();
            break;
    }
}

void WaveChannel::Step()
{
    if (--_timer <= 0) {
        _timer = (2048 - getTimer()) * 2;
        _posOnWave = (_posOnWave + 1) & 0x1F;
        if (IsRunning() && isDACEnabled()) {
            int position = _posOnWave / 2;
            uint8 volume = _getVolume();
            uint8 outputByte = _waveData[position];
            bool highBit = (_posOnWave & 0x1) == 0;
            if (highBit) {
                outputByte >>= 4;
            }
            outputByte &= 0xF;
            if (volume > 0) {
                outputByte >>= volume - 1;
            } else {
                outputByte = 0;
            }
            _outputVolume = outputByte;
        } else {
            _outputVolume = 0;
        }
    }
}

void WaveChannel::onTrigger()
{
    if (_lengthCountdown == 0) {
        _lengthCountdown = 256;
    }
    _timer = (2048 - getTimer()) * 2;
    _posOnWave = 0;
}

bool WaveChannel::isDACEnabled() const { return (_regs[0] & 0x80) == 0x80; }

uint8 WaveChannel::getLengthLoad() const { return _regs[1]; }