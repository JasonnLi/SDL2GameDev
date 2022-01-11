#include "../Core/Engine.h"
#include "../Graphics/TextureManager.h"
#include "../Inputs/Input.h"
#include "../Characters/Warrior.h"
#include "../Characters/Enemy.h"
#include <SDL2/SDL.h>
#include "../Timer/Timer.h"
#include "../Map/MapParser.h"
#include <iostream>
#include "../Camera/Camera.h"

Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr;

bool Engine::Init(){

    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Window = SDL_CreateWindow("SDL2 Console Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if(m_Window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_SOFTWARE);
    if(m_Renderer == nullptr){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    if(!MapParser::GetInstance()->Load()){
        std::cout << "Failed to load map" << std::endl;
        return false;
    }

    m_LevelMap = MapParser::GetInstance()->GetMap("level1");

    TextureManager::GetInstance()->ParseTextures("/textures.tml");

    Warrior* player = new Warrior(new Properties("player_idle", 100, 200, 136, 96));
    Enemy* boss = new Enemy(new Properties("boss_idle", 820, 240, 460, 352));

    m_GameObjects.push_back(player);
    m_GameObjects.push_back(boss);

    Camera::GetInstance()->SetTarget(player->GetOrigin());
    return m_IsRunning = true;
}

void Engine::Render(){
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    TextureManager::GetInstance()->Draw("bg", 0, 0, 2100, 1050, 1, 1, 0.4);
    m_LevelMap->Render();

    for(unsigned int i = 0; i != m_GameObjects.size(); i++)
        m_GameObjects[i]->Draw();

    SDL_RenderPresent(m_Renderer);
}

void Engine::Update(){
    float dt = Timer::GetInstance()->GetDeltaTime();
    for(unsigned int i = 0; i != m_GameObjects.size(); i++)
        m_GameObjects[i]->Update(dt);
    Camera::GetInstance()->Update(dt);
    m_LevelMap->Update();
}

void Engine::Events(){
    Input::GetInstance()->Listen();
}

bool Engine::Clean(){
    for(unsigned int i = 0; i != m_GameObjects.size(); i++)
        m_GameObjects[i]->Clean();

    TextureManager::GetInstance()->Clean();
    MapParser::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
}

void Engine::Quit(){
    m_IsRunning = false;
}
