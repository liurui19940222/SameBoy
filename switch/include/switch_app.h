#pragma once

#include <SDL.h>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include "../../gb-core/include/app.h"
#include "../../gb-core/include/datatype.h"

using namespace std;

const int SWTICH_JBT_UP = 13;
const int SWITCH_JBT_DOWN = 15;
const int SWITCH_JBT_LEFT = 12;
const int SWITCH_JBT_RIGHT = 14;
const int SWITCH_JBT_SELECT = 11;
const int SWITCH_JBT_START = 10;
const int SWITCH_JBT_X = 2;
const int SWITCH_JBT_Y = 3;
const int SWITCH_JBT_B = 1;
const int SWITCH_JBT_A = 0;

class SwitchApp : public App
{
public:
    SwitchApp(SDL_Renderer* renderer);
    virtual ~SwitchApp();

    void Render();

    void ProcessEvent(const SDL_Event& event);

private:
    SDL_Renderer* _renderer;

    SDL_Texture* _mainTex;
};

