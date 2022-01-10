#include "Timer.h"
#include <SDL2/SDL.h>

Timer* Timer::s_Instance = nullptr;

void Timer::Tick(){
    // Delta time is the difference in ticks from last frame (converted to seconds), e.g.
    // float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    // Update x position by 150 pixels/second, e.g.
    // mPosition.x += 150 * deltaTime;
    // Calculate number of frames that need to be rendered within a timeframe in millisecs
    m_DeltaTime = (SDL_GetTicks() - m_LastTime)*(TARGET_FPS/1000.0f);

    if(m_DeltaTime > TARGET_DELTATIME)
        m_DeltaTime = TARGET_DELTATIME;

    m_LastTime = SDL_GetTicks();
}
