#pragma once

#include "datatype.h"
#include "tools.h"

class Channel {
public:
    Channel();
    virtual ~Channel();
    virtual void Step();
    virtual uint8 Read(uint16 regIndex);
    virtual void Write(uint16 regIndex, uint8 data);
    void TickLength();
    void TickEnv();
    uint8 GetOutputVolume() const { return _outputVolume; }
    bool IsRunning() const { return _lengthCountdown > 0; }

protected:
    uint8 _regs[5] = {0};
    uint8 _curVolume = 0;
    uint8 _outputVolume = 0;
    uint16 _lengthCountdown = 0;
    int _timer = 0;
    int _curEnvPeriod = 0;
    bool _isEnvRunning = true;

    inline uint8 getVolume() const { return (_regs[2] >> 4) & 0xF; }
    inline bool isEnvAddMode() const { return (_regs[2] & 0x8) == 0x8; }
    inline uint8 getEnvPeriod() const { return (_regs[2] & 0x7); }
    inline bool isLengthEnabled() const { return (_regs[4] & 0x40) == 0x040; }
    uint16 getTimer() const;
    void setTimerLoad(uint16 value);
    virtual bool isDACEnabled() const;
    virtual uint8 getLengthLoad() const;
    virtual void onTrigger() {}
};
