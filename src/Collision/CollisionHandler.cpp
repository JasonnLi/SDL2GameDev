#include "../Collision/CollisionHandler.h"
#include "../Core/Engine.h"

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
    // the collision layer is put in the front, so using front to get the layer that has collision and check the mapping collision
    m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetLayers().front();
    m_CollisionTilemap = m_CollisionLayer->GetTileMap();
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b) {
    bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
    bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);
    return (x_overlaps && y_overlaps);
}

bool CollisionHandler::MapCollision(SDL_Rect a){
    // int tileSize = 32;
    // int RowCount = 20;
    // int ColCount = 60;
    int tileSize = 32;
    int RowCount = 30;
    int ColCount = 211;
    // if the player in  x = 320 postion, x / tileSize = 10, the player is in the 10th tile
    int left_tile = a.x/tileSize;
    int right_tile = (a.x + a.w)/tileSize;

    int top_tile = a.y/tileSize;
    int bottom_tile = (a.y + a.h)/tileSize;

    if(left_tile < 0) left_tile = 0;
    if(right_tile > ColCount) right_tile = ColCount;

    if(top_tile < 0) top_tile = 0;
    if(bottom_tile > RowCount) bottom_tile = RowCount;

    // It is checking if there is collision between the rect a and the ground
    for(int i = left_tile; i <= right_tile; ++i){
        for(int j = top_tile; j <= bottom_tile; ++j){
            // m_CollisionTilemap[j][i] represents tile id, if tileId = 0, it means there is no tile in the map
            if(m_CollisionTilemap[j][i] > 0){
                return true;
            }
        }
    }

    return false;
}
