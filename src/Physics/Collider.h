#ifndef COLLIDER_H
#define COLLIDER_H

#include <SDL2/SDL.h>

// Box to know if the it is hitting anything
class Collider {

    public:
        inline SDL_Rect Get(){return m_Box;}
        inline void SetBuffer(int x, int y, int w, int h){m_Buffer = {x, y, w, h};}

        void Set(int x, int y, int w, int h){
            m_Box = {
                x - m_Buffer.x,
                y - m_Buffer.y,
                w - m_Buffer.w,
                h - m_Buffer.h
            };
        }

    private:
        SDL_Rect m_Box;
        // Buffer is to used to make sure the box size is larger than the player
        SDL_Rect m_Buffer;
};

#endif // COLLIDER_H
