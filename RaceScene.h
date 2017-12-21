#ifndef RACE_SCENE_H
#define RACE_SCENE_H
#include "Scene.h"
#include "Car.h"
#include "Terrain.h"
#include "SkyDome.h"
#include "ThirdPersonCamera.h"
#include "Noob.h"
#include "gl_font.h"

#define NUMBER_OF_NOOBS 3
class RaceScene : public Scene
{
    ThirdPersonCamera m_camera;
    Car* m_player;
    BaseCar* m_noob;
    Terrain* m_terrain;
    GLFont m_font;
    SkyDome *m_sky;
    vector<Noob*> m_noobs;
    int m_numberOfNoobs;
    void GetNewNoob(Noob* &);
public:
    RaceScene(void);
    ~RaceScene(void);
    void Initialize();
    void LoadContent();
    void Update(unsigned p_elapsedTime);
    void Draw(unsigned p_elapsedTime);
    void DrawHUD();

};

#endif