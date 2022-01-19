#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Animation/SeqAnimation.h"

#define RUN_FORCE 5.0f

class Enemy: public Character {

    public:
        Enemy(Properties* props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);

    private:
        void AnimationState();

    private:
        bool m_IsRunning;
        bool m_IsGrounded;
        bool m_IsDead;

        Collider* m_Collider;
        RigidBody* m_RigidBody;
        SeqAnimation* m_Animation;
        Vector2D m_LastSafePosition;
};

#endif // ENEMY_H
