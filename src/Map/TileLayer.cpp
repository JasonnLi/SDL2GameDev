#include "TileLayer.h"
#include "../Graphics/TextureManager.h"

TileLayer::TileLayer(int tilesize, int colcount, int rowcount, TileMap tilemap, TilesetsList tilesets):
m_TileSize(tilesize), m_ColCount(colcount), m_RowCount(rowcount), m_Tilemap(tilemap), m_Tilesets(tilesets){

    for(unsigned int i=0; i < m_Tilesets.size(); i++)
        TextureManager::GetInstance()->Load(m_Tilesets[i].Name, "/maps/" + m_Tilesets[i].Source);
}

void TileLayer::Render(){
    for(unsigned int i = 0; i < m_RowCount; i++){
        for(unsigned int j = 0; j < m_ColCount; j++){

            int tileID = m_Tilemap[i][j];
            int temp = tileID;

            if(tileID == 0)
                continue;

            else{
                int index = 0;
                if(m_Tilesets.size() > 1){
                    for(unsigned int k = 1; k < m_Tilesets.size(); k++){
                        if(tileID > m_Tilesets[k].FirstID && tileID < m_Tilesets[k].LastID){
                            // algorithm used to calculate the correct tile id inside the current tileset
                            // i.e found the tile in the k tileset and the id is tile Id
                            // m_Tilesets[k].LastID = m_Tilesets[k].FirstID + m_Tilesets[k].TileCount
                            tileID = tileID + m_Tilesets[k].TileCount - m_Tilesets[k].LastID;
                            index = k;
                            break;
                        }
                    }
                }

                Tileset ts = m_Tilesets[index];
                // if tile id < ts.ColCount, int tileID/ts.ColCount = 0, thus, tile row is 0.
                // if tile id > ts.Colcount, e.g. id = 16, ColCount = 13, then tile row is 1.
                int tileRow = tileID/ts.ColCount;
                // minus 1 as tile id start from 0 in the tileset
                int tileCol = tileID - tileRow*ts.ColCount-1;

                // if this tile is on the las column
                if(tileID % ts.ColCount == 0){
                    tileRow--;
                    tileCol = ts.ColCount - 1;
                }

                TextureManager::GetInstance()->DrawTile(ts.Name, ts.TileSize, j*ts.TileSize, i*ts.TileSize, tileRow, tileCol);
            }
        }
    }
}

void TileLayer::Update(){

}
