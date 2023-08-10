#include "../include/apu.h"

#include <stdio.h>

APU::APU(MemoryBus *bus, std::function<void(const float *, int)> play)
{
    _play = play;
    _bus = bus;
	_smapleBuffer.resize(PCM_BUFFER_SIZE);
    _channels.push_back(make_shared<SquareChannel>());
    _channels.push_back(make_shared<SquareChannel>());
    _channels.push_back(make_shared<WaveChannel>());
    _channels.push_back(make_shared<NoiseChannel>());
}

void APU::Write(uint16 address, uint8 data)
{
    if (inRange(address, MEM_NR10, MEM_NR14)) {
        _channels[0]->Write(address - MEM_NR10, data);
    } else if (inRange(address, MEM_NR21, MEM_NR24)) {
        _channels[1]->Write(address - MEM_NR21 + 1, data);
    } else if (inRange(address, MEM_NR30, MEM_NR34)) {
        _channels[2]->Write(address - MEM_NR30, data);
    } else if (inRange(address, MEM_NR41, MEM_NR44)) {
        _channels[3]->Write(address - MEM_NR41 + 1, data);
    } else if (inRange(address, 0xFF30, 0xFF3F)) {
        std::static_pointer_cast<WaveChannel>(_channels[2])->WriteWave(address, data);
    } else if (address == MEM_NR50) {
        _nr50 = data;
    } else if (address == MEM_NR51) {
        _nr51 = data;
    } else if (address == MEM_NR52) {
        _nr52 = data;
    }
}

uint8 APU::Read(uint16 address)
{
    if (inRange(address, MEM_NR10, MEM_NR14)) {
        return _channels[0]->Read(address - MEM_NR10);
    } else if (inRange(address, MEM_NR21, MEM_NR24)) {
        return _channels[1]->Read(address - MEM_NR21 + 1);
    } else if (inRange(address, MEM_NR30, MEM_NR34)) {
        return _channels[2]->Read(address - MEM_NR30);
    } else if (inRange(address, MEM_NR41, MEM_NR44)) {
        return _channels[3]->Read(address - MEM_NR41 + 1);
    } else if (inRange(address, 0xFF30, 0xFF3F)) {
        return std::static_pointer_cast<WaveChannel>(_channels[2])->ReadWave(address);
    } else if (address == MEM_NR50) {
        return _nr50;
    } else if (address == MEM_NR51) {
        return _nr51;
    } else if (address == MEM_NR52) {
        uint8 data = _nr52;
        for (int i = 0; i < 4; ++i) {
            data |= _channels[i]->IsRunning() << i;
        }
        return data;
    }
    return 0xFF;
}

void APU::Step(int cycles)
{
    for (int i = 0; i < cycles; ++i) {
        _tick();
        _sample();
    }
}

void APU::_tick()
{
    if (++_mainClockCounter >= APU_CLOCK_PERIOD) {
        _mainClockCounter = 0;
        if (_divapu % 2 == 0) {
            for (int i = 0; i < 4; ++i) {
                _channels[i]->TickLength();
            }
        }
        if (_divapu == 2 || _divapu == 6) {
            std::static_pointer_cast<SquareChannel>(_channels[0])->TickSweep();
        }
        if (_divapu == 7) {
            for (int i = 0; i < 4; ++i) {
                if (i == 2) {
                    continue;
                }
                _channels[i]->TickEnv();
            }
        }
        _divapu = (_divapu + 1) % 8;
    }

    for (int i = 0; i < 4; ++i) {
        _channels[i]->Step();
    }
}

void APU::_sample()
{
    if (++_samplePeriodCounter >= PCM_SAMPLE_PERIOD) {
        _samplePeriodCounter = 0;

        float leftSample = 0, rightSample = 0, sample = 0;

        for (int i = 0; i < 4; ++i) {
            sample = ((float)(_channels[i]->GetOutputVolume())) / 100;
            if (_isLeftChannelEnabled(i)) {
                leftSample += sample;
            }
            if (_isRightChannelEnabled(i)) {
                rightSample += sample;
            }
        }

        _smapleBuffer.push_back(leftSample);
        _smapleBuffer.push_back(rightSample);
    }
    if (_smapleBuffer.size() >= PCM_BUFFER_SIZE) {
        _play(&(_smapleBuffer[0]), _smapleBuffer.size());
        _smapleBuffer.clear();
    }
}