#include "GameComponent.h"

void GameComponent::Enabled(bool p_enabled)
{
    m_enabled = p_enabled;
}

bool GameComponent::Enabled()
{
    return m_enabled;
}

GameComponent::GameComponent()
{
    m_enabled = true;
}

void GameComponent::Initialize()
{}


void GameComponent::Update(unsigned p_elapsedTime)
{}
