#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt){

    if(m_Target != nullptr){

        m_ViewBox.x = m_Target->X - SCREEN_WIDTH / 2;
        m_ViewBox.y = m_Target->Y - SCREEN_HEIGHT / 2;

        if(m_ViewBox.x < 0){
            m_ViewBox.x = 0;
        }

        if(m_ViewBox.y < 0){
            m_ViewBox.y = 0;
        }

        // set the camera to stop when it reach 7 times screen_width
        if(m_ViewBox.x > (7*SCREEN_WIDTH - m_ViewBox.w)){
            m_ViewBox.x = (7*SCREEN_WIDTH - m_ViewBox.w);
        }

        if(m_ViewBox.y > (SCREEN_HEIGHT - m_ViewBox.h)){
            m_ViewBox.y = (SCREEN_HEIGHT - m_ViewBox.h);
        }

        m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y);
    }
}

bool Camera::HitCamViewBox(SDL_Rect rTarget) {
    float leftBorder = rTarget.x;
    float rightBorder = rTarget.x + rTarget.w;

    if (leftBorder < m_ViewBox.x) {
        return true;
    }
    if (rightBorder > (m_ViewBox.x + m_ViewBox.w)) {
        return true;
    }
    return false;
}

