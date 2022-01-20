#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <map>
#include <string>
#include "../Map/GameMap.h"
#include "../Vendor/TinyXML/tinyxml.h"
#include "../Map/TileLayer.h"

enum EMapLevel {
	level1,
	level2,
};

class MapParser {

   public:
        bool Load();
        void Clean();

        inline GameMap* GetMap(EMapLevel id){return m_MapDict[id];}
        inline static MapParser* GetInstance(){ return s_Instance = (s_Instance != nullptr)? s_Instance : new MapParser();}
    
    private:
        MapParser(){}

        GameMap* createGameMap(EMapLevel id);
        bool Parse(EMapLevel id, std::string source);
        Tileset ParseTileset(TiXmlElement* xmlTileset);
        TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, std::vector<Tileset> tilesets, int tilesize, int rowcount, int colcount);

        static MapParser* s_Instance;
        std::map<EMapLevel, GameMap*> m_MapDict;
};

#endif // MAPPARSER_H
