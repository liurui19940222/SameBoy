#pragma once

#include <SDL2/SDL.h>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include "../../gb-core/include/app.h"
#include "../../gb-core/include/datatype.h"

using namespace std;

class WinApp : public App
{
public:
    WinApp(SDL_Renderer* renderer);
    virtual ~WinApp();

    void Render();

    void ProcessEvent(const SDL_Event& event);

private:
    SDL_Renderer* _renderer;

    SDL_Texture* _mainTex;
};

