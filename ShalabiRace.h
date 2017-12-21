#ifndef SHALABI_RACE_H
#define SHALABI_RACE_H

#include "DrawableGameComponent.h"
#include "Scene.h"
#include <vector>
using namespace std;

const float CAMERA_FOVX = 80.0f;
const float CAMERA_ZFAR = 16384;
const float CAMERA_ZNEAR = 1.0f;

class ShalabiRace : DrawableGameComponent
{
    Scene* m_currentScene;

public:
    const static int WindowWidth = 1024;
    const static int WindowHeight = 768;

    ShalabiRace(void);
    ~ShalabiRace(void);

    void Initialize();
    void LoadContent();
    void Draw(unsigned);
    void Update(unsigned);
};

#endif