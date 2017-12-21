#ifndef DRAWABLE_GAME_COMPONENT_H
#define DRAWABLE_GAME_COMPONENT_H

#include "GameComponent.h"

class DrawableGameComponent : public GameComponent
{
    bool m_visible;
public:

    DrawableGameComponent();

    void Visible(bool p_visible);
    bool Visible();

    void virtual LoadContent();
    void virtual Draw(unsigned p_elapsedTime);
};

#endif
