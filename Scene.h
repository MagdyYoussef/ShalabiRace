#ifndef SCENE_H
#define SCENE_H

#include "DrawableGameComponent.h"
#include <vector>
using namespace std;

class Scene : public DrawableGameComponent
{
protected:
    vector<DrawableGameComponent*> m_components;
public:
    Scene(void);
    ~Scene();

    void Initialize();
    void LoadContent();
    void Update(unsigned p_elapsedTime);
    void Draw(unsigned p_elapsedTime);
};

#endif