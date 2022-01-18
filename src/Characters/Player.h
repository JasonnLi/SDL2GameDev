#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Animation/SeqAnimation.h"
#include "../Collision/CollisionHandler.h"
#include "../Physics/Vector2D.h"


#define JUMP_TIME 40.0f;
#define JUMP_FORCE 20.0f

#define RUN_FORCE 8.0f
#define ATTACK_TIME 20.0f

#define PLAYER_WIDTH 24
#define PLAYER_HEIGHT 32

class Player: public Character {

    public:
        Player(Properties* props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);

    private:
        void AnimationState();

    private:
        bool m_IsRunning;
        bool m_IsJumping;
        bool m_IsFalling;
        bool m_IsGrounded;

        float m_JumpTime;
        float m_JumpForce;

        Collider* m_Collider;
        RigidBody* m_RigidBody;
        SeqAnimation* m_Animation;
        Vector2D m_LastSafePosition;
};

#endif // ENEMY_H
