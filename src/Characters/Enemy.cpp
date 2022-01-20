#include "Enemy.h"
#include "../Camera/Camera.h"
#include "../Collision/CollisionHandler.h"
#include "../Factory/ObjectFactory.h"
#include <iostream>

static Registrar<Enemy> registrar("ENEMY");

Enemy::Enemy(Properties* props): Character(props){
    m_Collider = new Collider();
    m_Collider->SetBuffer(2, 2, -5, -2);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(9.8f);

    m_Animation = new SeqAnimation(false);
    m_Animation->Parse("/marioSeries/enemyAnimation.aml");
    m_Animation->SetCurrentSeq("mushroom_idle");
}

void Enemy::Draw(){
    m_Animation->DrawFrame(m_Transform->X, m_Transform->Y, 1, 1, 1, m_Flip);
    // draw a bounding box for debug purpose
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);
}

void Enemy::Update(float dt){

    m_RigidBody->UnSetForce();

    // Y-Axis movements
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform->X, m_Transform->Y, 32, 32);

    // Run backward once it is grounded
    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_IsGrounded = true;
        m_Transform->Y = m_LastSafePosition.Y;
    } else {
        m_IsGrounded = false;
    }

    if (m_IsGrounded && Camera::GetInstance()->EnterCamera(m_Collider->Get())) {
        // start X-Axis movements when the character has collision with the camera
        m_RigidBody->ApplyForceX(BACKWARD*RUN_FORCE);
    }
    // X-Axis movements
    m_RigidBody->Update(dt);
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform->X, m_Transform->Y, 32, 32);

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    AnimationState();
    m_Animation->Update(dt);
    m_Animation->SetRepeat(true);
}

void Enemy::AnimationState(){
    // idling
    m_Animation->SetCurrentSeq("mushroom_idle");

    // running
    if(m_IsGrounded)
        m_Animation->SetCurrentSeq("mushroom_move");

    m_Animation->SetRepeat(true);
}


void Enemy::Clean(){

}



