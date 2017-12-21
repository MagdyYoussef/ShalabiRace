#pragma once

class GameComponent
{
    bool m_enabled;
public:
    
    void Enabled(bool p_enabled);
    bool Enabled();

    GameComponent();
    void virtual Initialize();
    void virtual Update(unsigned p_elapsedTime);

};
