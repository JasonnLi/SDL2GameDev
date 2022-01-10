#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include "Layer.h"

// GameMap class represets the map for the game which includes all layers in a Map
class GameMap {

    public:

        GameMap(){}

        void Render(){
            for(unsigned int i = 0; i < m_MapLayers.size(); i++)
                m_MapLayers[i]->Render();
        }

        void Update(){
            for(unsigned int i=0; i < m_MapLayers.size(); i++)
                m_MapLayers[i]->Update();
        }

        std::vector<Layer*> GetLayers(){
            return m_MapLayers;
        }

    private:
        friend class MapParser;
        std::vector<Layer*> m_MapLayers;
};

#endif // GAMEMAP_H
