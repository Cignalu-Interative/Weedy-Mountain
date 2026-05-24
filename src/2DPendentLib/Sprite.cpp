#include "pch.h"
#include "Sprite.h"
#include "Render.h"
#include "Window.h"
#include "AssetManager.h"
#include "Camera.h"

Sprite::~Sprite()
{
    if (m_texture != nullptr)
    {
        //SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
	}
}

void Sprite::Initialise(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Render* f_render)
{
	m_texture = AssetManager::Get()->GetTexture(p_id);

    int textureWidth, textureHeight;
    SDL_QueryTexture(m_texture, NULL, NULL, &textureWidth, &textureHeight);
    Vector2i spriteDimention = { textureWidth, textureHeight };
    SetDimension(spriteDimention);
    //AutoDetectFrameSize(spriteDimention);

    if (p_dimension.GetX() > 0 && p_dimension.GetY() > 0)
    {
        Resize(p_dimension);
    }

    //m_currentFrame = { 0.0f, 0.0f };
    int width = GetDimension().GetX();
    int height = GetDimension().GetY();
   // int frameW = m_frameSize.GetX();
    //int frameH = m_frameSize.GetY();

    float posX = GetPosition().GetX();
    float posY = GetPosition().GetY();
    m_spriteRect = { posX, posY, static_cast<float>(width), static_cast<float>(height) };

    SetPosition(p_position);
}
/*
void Sprite::Update(float p_deltaTimeMs)
{
    m_animTimer -= std::chrono::duration<float>(p_deltaTimeMs).count();
	m_speedAnimation = std::max(m_speedAnimation, 0.01f);  // Sécurité : pas de vitesse nulle ou négative

    if (m_animTimer <= m_speedAnimation)
    {
        // 1. Y A-T-IL DE LA VÉLOCITÉ ?
        if (HasVelocity())
        {
            //  WALK selon direction
            float velX = GetVelocityX();
            SetFacingRight(velX >= 0);  // Droite si velX >= 0

            // Animation WALK
            NextFrame(m_walkStartFrame, m_walkEndFrame);
        }
        else
        {
            //  IDLE (pas de vélocité)
            NextFrame(m_idleStartFrame, m_idleEndFrame);
        }
        m_animTimer = m_speedAnimation;  // Reset timer
    }
}

void Sprite::AutoDetectFrameSize(Vector2i p_spriteDimention)
{
    // Tailles frame courantes en game dev (power of 2)
    int commonSizes[] = { 8, 16, 24, 32, 48, 64, 96, 128 };
	int texWidth = p_spriteDimention.GetX();
	int texHeight = p_spriteDimention.GetY();

    for (int frameW : commonSizes)
    {
        if (texWidth % frameW != 0)  // Divise pile poil horizontalement
        {
            continue;
        }
        int frameH = texHeight / (texWidth / frameW);  // Proportion verticale
        if (texHeight % frameH != 0)  // Divise aussi verticalement
        {
            continue;
        }
        m_frameSize.SetX(frameW);
        m_frameSize.SetY(frameH);
        m_framesNB.SetX(texWidth / frameW);
        m_framesNB.SetY(texHeight / frameH);
        m_totalFrames = m_framesNB.GetX() * m_framesNB.GetY();
        return;
    }

    // Fallback : sprite unique
    m_frameSize.SetX(texWidth);
    m_frameSize.SetY(texHeight);
    m_framesNB.SetX(1);
    m_framesNB.SetY(1);
    m_totalFrames = 1;
}

void Sprite::SetCurrentFramePosition(Vector2f p_framePosition)
{
    m_currentFrame.SetX(std::min(p_framePosition.GetX(), m_framesNB.GetX() - 1));
    m_currentFrame.SetY(std::min(p_framePosition.GetY(), m_framesNB.GetY() - 1));
}

int Sprite::GetCurrentFrameIndex() const
{
    Vector2f pos = GetCurrentFramePosition();
    return (int)pos.GetY() * m_framesNB.GetX() + (int)pos.GetX();
}

void Sprite::NextFrame(int p_startFrame, int p_endFrame)
{
    // Par défaut : boucle sur TOUTES les frames
    if (p_endFrame == -1)
        p_endFrame = m_totalFrames - 1;

    int currentIndex = GetCurrentFrameIndex();  // Numéro frame actuel (0-31)

    // Frame suivante
    currentIndex++;

    // Boucle dans l'intervalle donné
    if (currentIndex > p_endFrame)
        currentIndex = p_startFrame;

    // Convertit index  position grille (X,Y)
    if (m_framesNB.GetX() != 0 && m_framesNB.GetY() != 0)
    {
        int frameX = currentIndex % (int)m_framesNB.GetX();
        int frameY = currentIndex / (int)m_framesNB.GetX();

        SetCurrentFramePosition({ (float)frameX, (float)frameY });
    }

}

void Sprite::SetFacingRight(bool p_facingRight)
{
    m_facingRight = p_facingRight;
}

bool Sprite::IsFacingRight() const
{
    return m_facingRight;
}

void Sprite::SetSpeedAnimation(float p_speed)
{
    // p_speed = 0.1f = 10 FPS, 0.05f = 20 FPS, etc.
    // Inversement proportionnel : plus p_speed est petit, plus l'animation est rapide
    m_speedAnimation = p_speed;  // Reset timer pour éviter des sauts d'animation
    m_animTimer = m_speedAnimation;
}
*/
void Sprite::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{
    SDL_Renderer* renderer = f_render->m_renderer;

    // Vérifications sécurité
    if (!m_texture || !renderer)
    {
        return;
    }

    // 1. SOURCE : Le morceau à découper dans la texture (ou NULL pour tout afficher)
    SDL_Rect* srcRect = nullptr;
    //if (m_totalFrames > 1)  // Sprite sheet ?
    //{
    //    srcRect = new SDL_Rect{
    //        static_cast<int>(m_currentFrame.GetX() * m_frameSize.GetX()),  // X dans sheet
    //        static_cast<int>(m_currentFrame.GetY() * m_frameSize.GetY()),  // Y dans sheet
    //        m_frameSize.GetX(),                                            // Largeur frame
    //        m_frameSize.GetY()                                             // Hauteur frame
    //    };
    //}

    // 2. DESTINATION : Où et quelle taille à l'écran

    Vector2f dimension;
    Vector2f position = GetPosition({ 0, 0 });

    float posX;
    float posY;
    if (IsGUI())
    {
        Vector2f dimension = (Vector2f)GetDimension();
        posX = position.GetX();
        posY = position.GetY();
	}
    else
    {
        dimension = (Vector2f)GetDimension() * p_camera->GetZoom();
        posX = p_camera->WorldToScreen(position).GetX();
		posY = p_camera->WorldToScreen(position).GetY();
    }
    
    Vector2i dimensionWindow = f_window->GetWindowSize();
    
    Vector2f screenPosBottomRight = {posX + dimension.GetX(), posY + dimension.GetY()};

    if (screenPosBottomRight.GetX() < 0 || posX > dimensionWindow.GetX() ||
        screenPosBottomRight.GetY() < 0 || posY > dimensionWindow.GetY())
    {
        //std::cerr << "no draw" << std::endl;
        return;
    }
    //std::cerr << dimension.GetX() << " " << dimension.GetY() << std::endl;

    float width = dimension.GetX();
    float height = dimension.GetY();
    SDL_FRect dstRect = { posX, posY, width, height };

    Vector2f rotPoint = GetRotationPoint();           // ex: (0,0) ou (0.5,0.5)
    SDL_FPoint rotationCenter{
        width * rotPoint.GetX(),                      // 0..width
        height * rotPoint.GetY()                       // 0..height
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    //if (HasVelocity())
    //{
    //    float velX = GetVelocityX();
    //    if (velX < 0)  // Va à gauche
    //        flip = SDL_FLIP_HORIZONTAL;
    //    else if (velX > 0)  // Va à droite
    //        flip = SDL_FLIP_NONE;
    //}

    // 3. Rendu avec ROTATION complète
    if (SDL_RenderCopyExF(renderer, m_texture,
        srcRect,           // Frame du sprite sheet (ou NULL)
        &dstRect,          // Position/taille à l'écran
        GetAngle(),        // Angle Transformable (degrés)
        &rotationCenter,   // Centre rotation pré-calculé
        flip) == -1)       //  miroir (optionnel)
    {
        SDL_Log("Lib/Sprite.cpp/Draw/SDL_RenderCopyExF/cela na pas fonctionné : %s", SDL_GetError());
		return;
    }

    // Nettoyage
    delete srcRect;
}