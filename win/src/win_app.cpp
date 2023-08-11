#include <iostream>
#include <thread>
#include <sstream>
#include <SDL2/SDL.h>
#include <memory>
#include "../include/win_app.h"
#include "../include/win_helper.h"
#include "../include/win_app.h"

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "SDL2.lib")

WinApp::WinApp(SDL_Renderer *renderer) : App(getCurrentDir() + "\\bin\\opcodes.txt")
{
    _renderer = renderer;
    _mainTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, GB_SCREEN_W, GB_SCREEN_H);

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

    std::string path = openFileDialog();
    if (!path.empty())
    {
        OpenRom(path.c_str());
    }
}

WinApp::~WinApp()
{
    if (_mainTex)
    {
        SDL_DestroyTexture(_mainTex);
    }
}

void WinApp::Render()
{
    // 锁定纹理并获取像素数据指针
    void *pixels;
    int pitch;
    SDL_LockTexture(_mainTex, NULL, &pixels, &pitch);

    // 修改像素数据
    Blit((uint32*)pixels);

    // 解锁纹理
    SDL_UnlockTexture(_mainTex);

    // 渲染纹理到屏幕上
    SDL_RenderCopy(_renderer, _mainTex, NULL, NULL);
    SDL_RenderPresent(_renderer);

    // 播放声音样本数据
    if (_needFlushSamples) {
        std::lock_guard<std::mutex> lock(_sampleMutex);
        SDL_QueueAudio(1, _soundData, _smaplesSize * sizeof(float));
        _needFlushSamples = false;
    }
}

void WinApp::ProcessEvent(const SDL_Event &event)
{
    Buttons button;
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_w:
            button = Buttons::Up;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            button = Buttons::Left;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            button = Buttons::Down;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            button = Buttons::Right;
            break;
        case SDLK_j:
            button = Buttons::B;
            break;
        case SDLK_k:
            button = Buttons::A;
            break;
        case SDLK_SPACE:
            button = Buttons::Select;
            break;
        case SDLK_RETURN:
            button = Buttons::Start;
            break;
        }
        if (_gb) _gb->JoypadPress(button);
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_w:
            button = Buttons::Up;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            button = Buttons::Left;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            button = Buttons::Down;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            button = Buttons::Right;
            break;
        case SDLK_j:
            button = Buttons::B;
            break;
        case SDLK_k:
            button = Buttons::A;
            break;
        case SDLK_SPACE:
            button = Buttons::Select;
            break;
        case SDLK_RETURN:
            button = Buttons::Start;
            break;
        }
        if (_gb) _gb->JoypadRelease(button);
        break;
    }
}

#undef main
int main(int argc, char* argv[]) {
    // 初始化 SDL
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口和渲染器
    SDL_Window* window = SDL_CreateWindow("SameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GB_SCREEN_W * 2, GB_SCREEN_H * 2, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // 禁用输入法
    SDL_StopTextInput();

    std::shared_ptr<WinApp> app = std::make_shared<WinApp>(renderer);
    std::chrono::steady_clock::time_point begin, end;
    std::chrono::milliseconds duration;
    // 等待用户关闭窗口
    bool quit = false;
    while (!quit) {
        begin = std::chrono::high_resolution_clock::now();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else {
                app->ProcessEvent(event);
            }
        }
        app->Render();
        end = std::chrono::high_resolution_clock::now();
	    duration = duration_cast<std::chrono::milliseconds>(end - begin);
        SDL_Delay(max(0, 16 - duration.count()));
    }

    // 清理资源并退出
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}