#pragma once

#include <stdint.h>
#include <stdio.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "channel.h"
#include "memory.h"
#include "noisechannel.h"
#include "squarechannel.h"
#include "wavechannel.h"

using namespace std;

const int PCM_BUFFER_SIZE = 4096;
const int PCM_SAMPLE_PERIOD = 4194304 / 44100;
const int APU_CLOCK_PERIOD = 4194304 / 512;

class APU : public IODevice {
public:
    APU(MemoryBus* bus, std::function<void(const float*, int)> play);
    virtual ~APU() {}

    void Write(uint16 address, uint8 value) override;
    uint8 Read(uint16 address) override;
    void Step(int cycles);

private:
    MemoryBus* _bus;

    uint8 _nr50, _nr51, _nr52;

    vector<shared_ptr<Channel>> _channels;

    std::function<void(const float*, int)> _play;

    int _mainClockCounter = 0, _samplePeriodCounter = 0, _divapu = 0;

    std::vector<float> _smapleBuffer;

    void _tick();
    void _sample();
    inline uint8 _getRightVolume() const { return _nr50 & 0b111; }
    inline uint8 _getLeftVolume() const { return _nr50 & 0b1110000; }
    inline bool _isLeftChannelEnabled(int channelIndex) const { return _nr51 & (1 << (channelIndex + 4)); }
    inline bool _isRightChannelEnabled(int channelIndex) const { return _nr51 & (1 << channelIndex); }
};
