#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <SDL2/SDL.h>
#include <vector>
#include "../Map/TileLayer.h"
#include "../Map/GameMap.h"

class CollisionHandler{

    public:
        // Check if there is collision happening
        bool MapCollision(SDL_Rect a);
        bool CheckCollision(SDL_Rect a, SDL_Rect b);

        inline static CollisionHandler* GetInstance(){ return s_Instance = (s_Instance != nullptr)? s_Instance : new CollisionHandler();}

    private:
        CollisionHandler();
        // Contain the map matrix that has each tile id
        TileMap m_CollisionTilemap;
        TileLayer* m_CollisionLayer;
        static CollisionHandler* s_Instance;
};

#endif // COLLISIONHANDLER_H
