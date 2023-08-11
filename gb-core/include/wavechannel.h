#pragma once

#include <vector>
#include "channel.h"
#include "datatype.h"

class WaveChannel : public Channel {
public:
    WaveChannel();
    void Write(uint16 regIndex, uint8 data) override;
    void Step() override;
    inline uint8 ReadWave(int waveIndex) { return _waveData[waveIndex]; }
    inline void WriteWave(int waveIndex, uint8 waveData) { _waveData[waveIndex] = waveData; }

protected:
    std::vector<uint8> _waveData;
    uint8 _posOnWave = 0;

    void onTrigger() override;
    bool isDACEnabled() const override;
	uint8 getLengthLoad() const override;

private:
    inline uint8 _getVolume() const { return (_regs[2] >> 5) & 0x3; }
};
