#include "DrawableGameComponent.h"



void DrawableGameComponent::Visible(bool p_visible)
{
    m_visible = p_visible;
}

bool DrawableGameComponent::Visible()
{
    return m_visible;
}

void DrawableGameComponent::LoadContent()
{}


DrawableGameComponent::DrawableGameComponent()
{
    m_visible = true;
}

void DrawableGameComponent::Draw(unsigned p_elapsedTime)
{
}
