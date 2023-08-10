#include <cstring>
#include <algorithm>
#include "../include/app.h"

App::App(string cfgPath)
{
    _cfgPath = cfgPath;
}

App::~App()
{
    CloseRom();
    if (_runThread)
    {
        _runThread->detach();
    }
    _runThread.reset();
}

void App::OpenRom(const char *romPath)
{
    if (romPath == NULL)
    {
        return;
    }
    if (_gb && !_runThreadAborted)
    {
        _shouldExit = true;
        while (!_runThreadAborted)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    _loadRom(romPath);
}

void App::CloseRom()
{
    _unloadRom();
}

void App::Blit(uint32 *target)
{
    std::lock_guard<std::mutex> lock(_pixelsMutex);
    memcpy(target, _frameData, 4 * GB_SCREEN_W * GB_SCREEN_H);
}

void App::_postRender(const uint32 *framebuffer)
{
    std::lock_guard<std::mutex> lock(_pixelsMutex);
    memcpy(_frameData, framebuffer, 4 * GB_SCREEN_W * GB_SCREEN_H);
}

void App::_postSound(const float *data, int size)
{
    std::lock_guard<std::mutex> lock(_sampleMutex);
    _smaplesSize = std::min(size, 4096);
    memcpy(_soundData, data, _smaplesSize * sizeof(float));
    _needFlushSamples = true;
}

void App::_loadRom(const char *romPath)
{
    _unloadRom();
    _shouldExit = false;
    auto draw = std::bind(&App::_postRender, this, std::placeholders::_1);
    auto play = std::bind(&App::_postSound, this, std::placeholders::_1, std::placeholders::_2);
    _gb = make_shared<Gameboy>();
    if (_gb->Setup(draw, play, romPath, _cfgPath.c_str()))
    {
        auto func = std::bind(&App::_run, this);
        if (_runThread)
        {
            _runThread->detach();
        }
        _runThread.reset();
        _runThread = make_shared<thread>(func);
    }
    else
    {
        _gb.reset();
    }
}

void App::_unloadRom()
{
    _shouldExit = true;
    if (_gb)
    {
        _gb.reset();
    }
}

void App::_run()
{
    _runThreadAborted = false;
    while (!_shouldExit)
    {
        _gb->Tick();
    }
    _runThreadAborted = true;
}