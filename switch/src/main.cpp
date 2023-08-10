#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <SDL.h>
#include <switch.h>
#include <algorithm>
#include <chrono>

#include "../include/switch_helper.h"
#include "../include/switch_app.h"

using namespace std;

int main(int argc, char *argv[])
{
    socketInitializeDefault();
    nxlinkStdio();
    romfsInit();
    chdir("romfs:/");

    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int w = 1280, h = 720;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        printf("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("sdl2_gles2", 0, 0, w, h, 0);
    if (!window)
    {
        printf("SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    for (int i = 0; i < 2; i++)
    {
        if (SDL_JoystickOpen(i) == NULL)
        {
            printf("SDL_JoystickOpen: %s\n", SDL_GetError());
            SDL_Quit();
            return -1;
        }
    }

    std::shared_ptr<SwitchApp> app = std::make_shared<SwitchApp>(renderer);
    std::chrono::system_clock::time_point begin, end;
    int duration;
    // 等待用户关闭窗口
    bool selectDown = false, startDown = false;
    while (!selectDown || !startDown)
    {
        begin = std::chrono::high_resolution_clock::now();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_JOYBUTTONDOWN:
                switch (event.jbutton.button)
                {
                case SWITCH_JBT_SELECT:
                    selectDown = true;
                    break;
                case SWITCH_JBT_START:
                    startDown = true;
                    break;
                }
                break;
            case SDL_JOYBUTTONUP:
                switch (event.jbutton.button)
                {
                case SWITCH_JBT_SELECT:
                    selectDown = false;
                    break;
                case SWITCH_JBT_START:
                    startDown = false;
                    break;
                }
                break;
            }
            app->ProcessEvent(event);
        }
        app->Render();
        end = std::chrono::high_resolution_clock::now();
        duration = duration_cast<std::chrono::milliseconds>(end - begin).count();
        SDL_Delay(max(0, 16 - duration));
    }

    socketExit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
