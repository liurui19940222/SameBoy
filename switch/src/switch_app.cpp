#include <iostream>
#include <thread>
#include <sstream>
#include <SDL.h>
#include "../include/switch_app.h"
#include "../include/switch_helper.h"

SwitchApp::SwitchApp(SDL_Renderer *renderer) : App(string("opcodes.txt"))
{
    // Set up SDL audio spec
	SDL_AudioSpec audioSpec;
	audioSpec.freq = 44100;
	audioSpec.format = AUDIO_F32SYS;
	audioSpec.channels = 2;
	audioSpec.samples = PCM_BUFFER_SIZE;	// Adjust as needed
	audioSpec.callback = NULL;
	audioSpec.userdata = this;

	SDL_AudioSpec obtainedSpec;
	SDL_OpenAudio(&audioSpec, &obtainedSpec);
	SDL_PauseAudio(0);

    _renderer = renderer;
    _mainTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, GB_SCREEN_W, GB_SCREEN_H);
    OpenRom("Super Mario Land (World).gb");
}

SwitchApp::~SwitchApp()
{
    if (_mainTex)
    {
        SDL_DestroyTexture(_mainTex);
    }
}

void SwitchApp::Render()
{
    static SDL_Rect rect = {(1280 - 800) / 2, (720 - 720) / 2, 800, 720};

    // 锁定纹理并获取像素数据指针
    void *pixels;
    int pitch;
    SDL_LockTexture(_mainTex, NULL, &pixels, &pitch);

    // 修改像素数据
    Blit((uint32 *)pixels);

    // 解锁纹理
    SDL_UnlockTexture(_mainTex);

    // 渲染纹理到屏幕上
    SDL_RenderCopy(_renderer, _mainTex, NULL, &rect);
    SDL_RenderPresent(_renderer);

    // 播放声音样本数据
    if (_needFlushSamples)
    {
        std::lock_guard<std::mutex> lock(_sampleMutex);
        SDL_QueueAudio(1, _soundData, _smaplesSize * sizeof(float));
        _needFlushSamples = false;
    }
}

void SwitchApp::ProcessEvent(const SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_JOYBUTTONDOWN:
        if (event.jbutton.which == 0)
        {
            Buttons button = Buttons::Select;
            switch (event.jbutton.button)
            {
            case SWTICH_JBT_UP:
                button = Buttons::Up;
                break;
            case SWITCH_JBT_LEFT:
                button = Buttons::Left;
                break;
            case SWITCH_JBT_DOWN:
                button = Buttons::Down;
                break;
            case SWITCH_JBT_RIGHT:
                button = Buttons::Right;
                break;
            case SWITCH_JBT_B:
                button = Buttons::B;
                break;
            case SWITCH_JBT_A:
                button = Buttons::A;
                break;
            case SWITCH_JBT_SELECT:
                button = Buttons::Select;
                break;
            case SWITCH_JBT_START:
                button = Buttons::Start;
                break;
            }
            if (_gb)
            {
                _gb->JoypadPress(button);
            }
        }
        break;
    case SDL_JOYBUTTONUP:
        if (event.jbutton.which == 0)
        {
            Buttons button = Buttons::Select;
            switch (event.jbutton.button)
            {
            case SWTICH_JBT_UP:
                button = Buttons::Up;
                break;
            case SWITCH_JBT_LEFT:
                button = Buttons::Left;
                break;
            case SWITCH_JBT_DOWN:
                button = Buttons::Down;
                break;
            case SWITCH_JBT_RIGHT:
                button = Buttons::Right;
                break;
            case SWITCH_JBT_B:
                button = Buttons::B;
                break;
            case SWITCH_JBT_A:
                button = Buttons::A;
                break;
            case SWITCH_JBT_SELECT:
                button = Buttons::Select;
                break;
            case SWITCH_JBT_START:
                button = Buttons::Start;
                break;
            }
            if (_gb)
            {
                _gb->JoypadRelease(button);
            }
        }
        break;
    }
}