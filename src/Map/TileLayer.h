#ifndef TILELAYER_H
#define TILELAYER_H

#include <string>
#include <vector>
#include "Layer.h"
#include "../Physics/Vector2D.h"

struct Tileset{
    int FirstID, LastID;
    int RowCount, ColCount;
    int TileCount, TileSize;
    std::string Name, Source;
};

// Contain a list of different tilesets
using TilesetsList = std::vector<Tileset> ;
// A vector matrix used to represent the tile map, which contains each tile id
using TileMap = std::vector<std::vector<int> >;

// A TileLayer is one of the layer inside the game map
class TileLayer : public Layer{

    public:
        TileLayer(int tilesize, int width, int height, TileMap tilemap, TilesetsList tilesets);

        virtual void Render();
        virtual void Update();
        inline TileMap GetTileMap(){return m_Tilemap;}

    private:
        int m_TileSize;
        int m_ColCount, m_RowCount;

        TileMap m_Tilemap;
        TilesetsList m_Tilesets;
};

#endif // TILELAYER_H
