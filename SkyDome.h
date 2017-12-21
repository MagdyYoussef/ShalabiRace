
#ifndef SKY_DOME_H
#define SKY_DOME_H

#include <math.h>
#include "DrawableGameComponent.h"

#define DEFAULT_DTHETA 15
#define DEFAULT_DPHI 15
#define DEFAULT_RADIUS 840
#define PI 3.1415926535897f
#define DTOR (PI / 180.0f)

class SkyDome : public DrawableGameComponent
{
public:
    struct Vertex
    {
        // 3d coordinates
        float x;
        float y;
        float z;

        // tex coordinates
        float u;
        float v;
    };

private:
    float m_radius;
    int m_dTheta;
    int m_dPhi;
    int m_skyTexId;
    int m_numberOfVertices;
    Vertex *m_vertices;
    void GenerateHemiShperePoints();
    void FixSeamProblem();

public:
    SkyDome(int p_skyTexId);
    SkyDome(float p_radius, int p_dTheta, int p_dPhi, int p_skyTexId);
    void Initialize();
    void Draw(unsigned p_elapsedTime);

    ~SkyDome(void);

};

#endif