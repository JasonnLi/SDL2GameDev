#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Animation/SeqAnimation.h"

class Enemy: public Character {

    public:
        Enemy(Properties* props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);

    private:
        Collider* m_Collider;
        RigidBody* m_RigiBody;
        SeqAnimation* m_Animation;
        Vector2D m_LastSafePosition;
};

#endif // ENEMY_H
