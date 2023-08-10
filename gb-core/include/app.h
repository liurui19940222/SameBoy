#pragma once

#include <memory>
#include <mutex>
#include <string>
#include "gameboy.h"

using namespace std;

class App {
public:
    App(string cfgPath);
    virtual ~App();
    void OpenRom(const char* romPath);
    void CloseRom();
    void Blit(uint32* target);

protected:
    shared_ptr<Gameboy> _gb;

    float _soundData[4096];

    bool _needFlushSamples = false;

    mutex _sampleMutex;

    int _smaplesSize = 0;

private:
    shared_ptr<thread> _runThread;

    mutex _pixelsMutex;

    bool _shouldExit = false;

    bool _runThreadAborted = false;

    uint32 _frameData[4 * GB_SCREEN_W * GB_SCREEN_H];

    string _cfgPath;

    void _postRender(const uint32* framebuffer);

    void _postSound(const float* data, int size);

    void _loadRom(const char* romPath);

    void _unloadRom();

    void _run();
};