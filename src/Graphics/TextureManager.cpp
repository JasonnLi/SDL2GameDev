#include "../Graphics/TextureManager.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"
#include "../Vendor/TinyXML/tinyxml.h"

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string fileName){
    SDL_Surface* surface = nullptr;

    m_SrcFile = "../../assets" + fileName;
    
    if (fileName.substr(fileName.find(".") + 1) == "bmp") {
        std::cout << "loading bmp file: " << m_SrcFile << std::endl;
        surface = SDL_LoadBMP(m_SrcFile.c_str());

        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
    } else {
        std::cout << "loading other format image file: " << m_SrcFile << std::endl;
        surface = IMG_Load(m_SrcFile.c_str());
    };

    if(surface == nullptr){
        SDL_Log("Failed to load texture: %s, %s", m_SrcFile.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
    if(texture == nullptr){
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return false;
    }

    m_TextureMap[id] = texture;

    SDL_FreeSurface(surface);

    return true;
}

void TextureManager::Draw(std::string id, int x, int y, int width, int heigt, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip){
    SDL_Rect srcRect = {0, 0, width, heigt};
    // scrollRatio is used to control the scroll speed of the rendered
    Vector2D cam = Camera::GetInstance()->GetPosition()*scrollRatio;
    SDL_Rect dstRect = {static_cast<int>(x - cam.X), static_cast<int>(y - cam.Y), static_cast<int>(width*scaleX), static_cast<int>(heigt*scaleY)};

    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int heigt, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = {width*frame, heigt*(row-1), width, heigt};
    
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {static_cast<int>(x - cam.X), static_cast<int>(y - cam.Y), width, heigt};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = {tileSize*frame, tileSize*row, tileSize, tileSize};

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {static_cast<int>(x - cam.X), static_cast<int>(y - cam.Y), tileSize, tileSize};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, 0, flip);
}

// Parse all defined textures in the the xml file
bool TextureManager::ParseTextures(std::string source){
    source = "../../assets" + source;
    TiXmlDocument xml;
    xml.LoadFile(source);
    if(xml.Error()){
        std::cout << "Failed to load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    for(TiXmlElement* e=root->FirstChildElement(); e!= nullptr; e=e->NextSiblingElement()){
        if(e->Value() == std::string("texture")){
            std::string id = e->Attribute("id");
            std::string src = e->Attribute("source");
            Load(id, src);
        }
    }

    std::cout << "Textures Parse success!" << std::endl;
    return true;
}

void TextureManager::Drop(std::string id){
    SDL_DestroyTexture(m_TextureMap[id]);
    m_TextureMap.erase(id);
}

void TextureManager::Clean(){
    std::map<std::string, SDL_Texture*>::iterator it;
    for(it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
        SDL_DestroyTexture(it->second);

    m_TextureMap.clear();

    SDL_Log("texture map cleaned!");
}

