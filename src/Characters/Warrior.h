#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "../Animation/SpriteAnimation.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Collision/CollisionHandler.h"
#include "../Physics/Vector2D.h"

#define JUMP_TIME 30.0f;
#define JUMP_FORCE 15.0f

#define RUN_FORCE 8.0f
#define ATTACK_TIME 20.0f

class Warrior: public Character{
    public:
        Warrior(Properties* props);
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
        bool m_IsAttacking;
        bool m_IsCrouching;

        float m_JumpTime;
        float m_JumpForce;
        // the time that the player needed to animate the attack
        float m_AttackTime;

        Collider* m_Collider;
        SpriteAnimation* m_Animation;
        RigidBody* m_RigidBody;
        // Make sure the player get back to the last safe position when they hit the wall (have collision)
        Vector2D m_LastSafePosition;
};

#endif // WARRIOR_H
