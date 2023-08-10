#pragma once

#include "channel.h"
#include "datatype.h"

class WaveChannel : public Channel {
public:
    void Write(uint16 regIndex, uint8 data) override;
    void Step() override;
    inline uint8 ReadWave(uint16 waveIndex) { return _waveData[waveIndex]; }
    inline void WriteWave(uint16 waveIndex, uint8 waveData) { _waveData[waveIndex] = waveData; }

protected:
    uint8 _waveData[16] = {0};
    uint8 _posOnWave = 0;

    void onTrigger() override;
    bool isDACEnabled() const override;
	uint8 getLengthLoad() const override;

private:
    inline uint8 _getVolume() const { return (_regs[2] >> 5) & 0x3; }
};
