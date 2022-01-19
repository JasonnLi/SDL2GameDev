#include "Player.h"
#include "../Camera/Camera.h"
#include "../Inputs/Input.h"
#include "../Factory/ObjectFactory.h"
#include "../Graphics/TextureManager.h"
#include <iostream>

static Registrar<Player> registrar("PLAYER");

Player::Player(Properties* props): Character(props){
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;

    m_Collider = new Collider();
    m_Collider->SetBuffer(2, 6, -5, -6);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(9.8f);

    m_Animation = new SeqAnimation(false);
    m_Animation->Parse("/marioSeries/animation.aml");
    m_Animation->SetCurrentSeq("mario_idle");
}

void Player::Draw(){
    m_Animation->DrawFrame(m_Transform->X, m_Transform->Y, 1, 1, 1, m_Flip);
    // draw a bounding box for debug purpose
    // Vector2D cam = Camera::GetInstance()->GetPosition();
    // SDL_Rect box = m_Collider->Get();
    // box.x -= cam.X;
    // box.y -= cam.Y;
    // SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);
}

void Player::Update(float dt){
    m_IsRunning = false;
    m_RigidBody->UnSetForce();
    // Run forward
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD){
        m_RigidBody->ApplyForceX(FORWARD*RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }

    // Run backward
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD){
        m_RigidBody->ApplyForceX(BACKWARD*RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }

    // Jump
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_J) && m_IsGrounded){
        m_IsJumping = true;
        m_IsGrounded = false;
        m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
    }
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_J) && m_IsJumping && m_JumpTime > 0){
        m_JumpTime -= dt;
        m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
    } else{
        m_IsJumping = false;
        m_JumpTime = JUMP_TIME;
    }

    // Fall state is judged by know if the Y velocity is greater than 0 and not grounding
    if(m_RigidBody->Velocity().Y > 0 && !m_IsGrounded)
        m_IsFalling = true;
    else
        m_IsFalling = false;

    // X-Axis movements
    m_RigidBody->Update(dt);
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform->X, m_Transform->Y, PLAYER_WIDTH, PLAYER_HEIGHT);

    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()))
        m_Transform->X = m_LastSafePosition.X;

    if(Camera::GetInstance()->HitCamViewBox(m_Collider->Get()))
        m_Transform->X = m_LastSafePosition.X;
    
    // Y-Axis movements
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform->X, m_Transform->Y, PLAYER_WIDTH, PLAYER_HEIGHT);

    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_IsGrounded = true;
        m_Transform->Y = m_LastSafePosition.Y;
    } else {
        m_IsGrounded = false;
    }

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    AnimationState();
    m_Animation->Update(dt);
    m_Animation->SetRepeat(true);

    // if(m_Animation->IsEnded()){
    //     m_Animation->SetCurrentSeq("boss_idle");
    //     m_Animation->SetRepeat(true);
    // }
}

void Player::AnimationState(){
    // idling
    m_Animation->SetCurrentSeq("mario_idle");

    // running
    if(m_IsRunning)
        m_Animation->SetCurrentSeq("mario_move");

    // jumping
    if(m_IsJumping)
         m_Animation->SetCurrentSeq("mario_jump");
    
        // jumping
    if(m_IsFalling && !m_IsGrounded)
         m_Animation->SetCurrentSeq("mario_jump");

    m_Animation->SetRepeat(true);

}


void Player::Clean(){
    TextureManager::GetInstance()->Drop(m_TextureID);
}



