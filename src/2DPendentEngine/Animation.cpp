#include "pch.h"
#include "Animation.h"
#include "Sprite.h"
#include "ParserSpriteSheet.h"

void Animation::Init(Sprite* p_sprite, const std::string& p_nameAnimation, bool p_loop,const std::string& p_infoAnimationSpriteSheet)
{
    m_sprite = p_sprite;
    m_currentTimer = m_frameTime;
    m_nameAnimation = p_nameAnimation;
    m_loop = p_loop;

    // Auto-detect frame size based on sprite dimensions
    if (p_infoAnimationSpriteSheet.empty())
        std::cout << "no Json for animation " << std::endl;

	m_parser = new ParserSpriteSheet();
    m_parser->ReadJsonYuki(p_infoAnimationSpriteSheet);
    m_frameSize = m_parser->GetFrameDimension();
    m_totalFrames = m_parser->GetInfoAnimationByName(p_nameAnimation)->s_nbFrames.size();
    m_frameTime = m_parser->GetInfoAnimationByName(p_nameAnimation)->s_frameTime;
        //m_sprite->GetSpriteRect() = new SDL_Rect();
		//m_infoAnimation = m_parser->GetInfoAnimationByName(p_nameAnimation);
}

void Animation::Update(float p_deltaTime)
{
    if (m_hasAnimation == false)
        return;

    m_currentTimer -= p_deltaTime;
    //int nbFrames = nameAnimation->s_nbFrames;

    if (m_currentTimer <= 0.f)
    {
        m_currentTimer += m_frameTime;
        m_currentFrame = (m_currentFrame + 1) % m_totalFrames;

		int spriteColumns = m_sprite->GetDimension().GetX() / m_frameSize.GetX();

        // recuperer l emplacement de l image sur le sprite source, recup le int dans le vector du parser 
        int frameInSprite = m_parser->GetInfoAnimationByName(m_nameAnimation)->s_nbFrames[m_currentFrame];

		float frameX = (frameInSprite % spriteColumns) * m_frameSize.GetX();
		float frameY = (frameInSprite / spriteColumns) * m_frameSize.GetY();
		//float frameX = m_currentFrame * m_frameSize.GetX();
		//float frameY = m_currentRow * m_frameSize.GetY();

        SDL_Rect* srcRect = m_sprite->GetSpriteRect();

        
        if (srcRect == nullptr)
        {
            srcRect = new SDL_Rect();
        }

         srcRect->x = static_cast<int>(frameX);
         srcRect->y = static_cast<int>(frameY);
         srcRect->w = m_frameSize.GetX();
         srcRect->h = m_frameSize.GetY();

         m_sprite->SetSourceRectSprite(*srcRect); 

    }
}

void Animation::AutoDetectFrameSize(Vector2i p_spriteDimention)
{
    // Tailles frame courantes en game dev (power of 2)
    //int commonSizes[] = { 8, 16, 24, 32, 48, 64, 96, 128 };
    //int texWidth = p_spriteDimention.GetX();
    //int texHeight = p_spriteDimention.GetY();

    //for (int frameW : commonSizes)
    //{
    //    if (texWidth % frameW != 0)  // Divise pile poil horizontalement
    //    {
    //        continue;
    //    }
    //    int frameH = texHeight / (texWidth / frameW);  // Proportion verticale
    //    if (texHeight % frameH != 0)  // Divise aussi verticalement
    //    {
    //        continue;
    //    }
    //    m_frameSize.SetX(frameW);
    //    m_frameSize.SetY(frameH);
    //    m_framesNB.SetX(texWidth / frameW);
    //    m_framesNB.SetY(texHeight / frameH);
    //    //m_totalFrames = m_framesNB.GetX() * m_framesNB.GetY();
    //    return;
    //}

    //// Fallback : sprite unique
    //m_frameSize.SetX(texWidth);
    //m_frameSize.SetY(texHeight);
    //m_framesNB.SetX(1);
    //m_framesNB.SetY(1);
    //m_totalFrames = 1;
}

void Animation::SetAnimationByName(const std::string& p_name)
{
    if (m_parser)
    {
		m_nameAnimation = p_name;
		m_frameSize = m_parser->GetFrameDimension();
    }
}
