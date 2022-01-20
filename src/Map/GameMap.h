#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include "Layer.h"
#include "../Object/GameObject.h"

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

        inline void addGameObject(GameObject* object) { m_GameObjects.push_back(object); }

        std::vector<Layer*> GetLayers(){
            return m_MapLayers;
        }

        inline std::vector<GameObject*> GetMapObjects(){ return m_GameObjects; }

    private:
        friend class MapParser;
        std::vector<Layer*> m_MapLayers;

        // store all game objects in this particular game map
        std::vector<GameObject*> m_GameObjects;
};

#endif // GAMEMAP_H
