#include "MapParser.h"
#include "../Factory/ObjectFactory.h"
#include "../Camera/Camera.h"

MapParser* MapParser::s_Instance = nullptr;

// @TODO load all levels of maps
bool MapParser::Load(){
    return Parse(level1, "../../assets/maps/map_lv11.tmx");
}

bool MapParser::Parse(EMapLevel id, std::string source){

    TiXmlDocument xml;
    xml.LoadFile(source);
    if(xml.Error()){
        std::cerr << "Failed to load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();

    int colcount, rowcount, tilesize = 0;
    root->Attribute("width", &colcount);
    root->Attribute("height", &rowcount);
    root->Attribute("tilewidth", &tilesize);

    // Parse Tile sets
    TilesetsList tilesets;
    for(TiXmlElement* e=root->FirstChildElement(); e!= nullptr; e=e->NextSiblingElement()){
        if(e->Value() == std::string("tileset")){
            tilesets.push_back(ParseTileset(e));
        }
    }

    GameMap* gameMap = createGameMap(level1);
    // Parse Layers
    for(TiXmlElement* e=root->FirstChildElement(); e!= nullptr; e=e->NextSiblingElement()){
        if(e->Value() == std::string("layer")){
            TileLayer* tilelayer = ParseTileLayer(e, tilesets, tilesize, rowcount, colcount);
            gameMap->m_MapLayers.push_back(tilelayer);
        }
    }

    m_MapDict[id] = gameMap;
    return true;
}

GameMap* MapParser::createGameMap(EMapLevel id) {
    GameMap* gameMap = new GameMap();
    GameObject* player = ObjectFactory::GetInstance()->CreateObject("PLAYER", new Properties("mario_idle", 100, 0, 24, 32));
    Camera::GetInstance()->SetTarget(player->GetOrigin());
    gameMap->addGameObject(player);
    switch (id)
    {
        case level1:
            GameObject* mushroom_0 = ObjectFactory::GetInstance()->CreateObject("ENEMY", new Properties("mushroom_idle", 864, 384, 32, 32));
            GameObject* mushroom_1 = ObjectFactory::GetInstance()->CreateObject("ENEMY", new Properties("mushroom_idle", 3680, 384, 32, 32));
            gameMap->addGameObject(mushroom_0);
            gameMap->addGameObject(mushroom_1);
            break;
    }
    return gameMap;
}

TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TilesetsList tilesets, int tilesize, int rowcount, int colcount){

    TiXmlElement* data;
    for(TiXmlElement* e=xmlLayer->FirstChildElement(); e!= nullptr; e=e->NextSiblingElement()){
        if(e->Value() == std::string("data")){
            data = e;
            break;
        }
    }

    // Parse Layer tile map
    std::string matrix(data->GetText());
    std::istringstream iss(matrix);
    std::string id;

    TileMap tilemap(rowcount, std::vector<int> (colcount, 0));
    for(int row = 0; row < rowcount; row++){
        for (int col = 0; col < colcount; col++){
            getline(iss, id, ',');
            std::stringstream convertor(id);
            convertor >> tilemap[row][col];

            if(!iss.good())
                break;
        }
    }

    return (new TileLayer(tilesize, colcount, rowcount, tilemap, tilesets));
}

Tileset MapParser::ParseTileset(TiXmlElement* xmlTileset){
    Tileset tileset;
    tileset.Name = xmlTileset->Attribute("name");
    xmlTileset->Attribute("firstgid", &tileset.FirstID);
    xmlTileset->Attribute("tilecount", &tileset.TileCount);
    tileset.LastID = (tileset.FirstID + tileset.TileCount) - 1;

    xmlTileset->Attribute("columns", &tileset.ColCount);
    tileset.RowCount = tileset.TileCount/tileset.ColCount;
    xmlTileset->Attribute("tilewidth", &tileset.TileSize);

    TiXmlElement* image = xmlTileset->FirstChildElement();
    tileset.Source = image->Attribute("source");
    return tileset;
}

void MapParser::Clean(){
    std::map<EMapLevel, GameMap*>::iterator it;
    for(it = m_MapDict.begin(); it != m_MapDict.end(); it++)
        it->second = nullptr;

    m_MapDict.clear();
}

