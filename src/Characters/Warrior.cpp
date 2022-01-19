#include "Warrior.h"
#include "../Graphics/TextureManager.h"
#include <SDL2/SDL.h>
#include "../Inputs/Input.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"
#include "../Factory/ObjectFactory.h"

static Registrar<Warrior> registrar("WARRIOR");

Warrior::Warrior(Properties* props): Character(props){

    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    m_AttackTime = ATTACK_TIME;

    m_Collider = new Collider();
    // Adjust the buffer to make your player attached on the ground
    m_Collider->SetBuffer(-38, -10, 38, 30);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(6.5f);

    m_Animation = new SpriteAnimation();
    // m_Animation->SetProps(m_TextureID, 1, 6, 100);
}

void Warrior::Draw(){
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, 1, 1, m_Flip);
    // get the camera target position (which is the warrior)
    // draw a box wrapping the player and make it follow the camera moving. It is for debug purpose (adjusting player collsion space)
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);
}

void Warrior::Update(float dt){
    m_IsRunning = false;
    m_IsCrouching = false;
    m_RigidBody->UnSetForce();

    // Run forward
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD && !m_IsAttacking){
        m_RigidBody->ApplyForceX(FORWARD*RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }

    // Run backward
    if(Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD && !m_IsAttacking){
        m_RigidBody->ApplyForceX(BACKWARD*RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }

    // Crouch
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S)){
        m_RigidBody->UnSetForce();
        m_IsCrouching = true;
    }

    // Attack
    if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)){
        m_RigidBody->UnSetForce();
        m_IsAttacking = true;
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
    }
    else{
        m_IsJumping = false;
        m_JumpTime = JUMP_TIME;
    }

    // Fall state is judged by know if the Y velocity is greater than 0 and not grounding
    if(m_RigidBody->Velocity().Y > 0 && !m_IsGrounded)
        m_IsFalling = true;
    else
        m_IsFalling = false;

    // Attack timer
    if(m_IsAttacking && m_AttackTime > 0){
        m_AttackTime -= dt;
    }
    else{
        m_IsAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }

    // move on X axis
    m_RigidBody->Update(dt);
    // Before move the next, set the current position as the last safe position
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    // Check the difference in the width and height setting
    // 96 96?
    m_Collider->Set(m_Transform->X, m_Transform->Y, 96, 96);
    // when there is collision, bring the player back to the last safe position
    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()))
        m_Transform->X = m_LastSafePosition.X;


    // move on Y axis
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform->X, m_Transform->Y, 96, 96);

    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())){
        m_IsGrounded = true;
        m_Transform->Y = m_LastSafePosition.Y;
    }
    else{
        m_IsGrounded = false;
    }

    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    AnimationState();
    m_Animation->Update(dt);
}

void Warrior::AnimationState(){
    // idling
    m_Animation->SetProps("player_idle", 1, 6, 100);

    // running
    if(m_IsRunning)
        m_Animation->SetProps("player_run", 1, 8, 100);

    // crouching
    if(m_IsCrouching)
        m_Animation->SetProps("player_crouch", 1, 6, 200);

    // jumping
    if(m_IsJumping)
         m_Animation->SetProps("player_jump", 1, 2, 200);

    // falling
    if(m_IsFalling)
         m_Animation->SetProps("player_fall", 1, 2, 350);

    // attacking
    if(m_IsAttacking)
        m_Animation->SetProps("player_attack", 1, 14, 80);
}

void Warrior::Clean(){
    // drop the texture to clean up the program as each texture is a spriteSheet
    TextureManager::GetInstance()->Drop(m_TextureID);
}

