#include "SkyDome.h"
#include <windows.h>
#include <gl/glut.h>



SkyDome::SkyDome(int p_skyTexId)
{
    m_radius = DEFAULT_RADIUS;
    m_dTheta = DEFAULT_DTHETA;
    m_dPhi = DEFAULT_DPHI;
    m_skyTexId = p_skyTexId;
    m_vertices = 0;
}


SkyDome::~SkyDome(void)
{
}

SkyDome::SkyDome(float p_radius, int p_dTheta, int p_dPhi, int p_skyTexId)
{
    m_radius = p_radius;
    m_dTheta = p_dTheta;
    m_dPhi = p_dPhi;
    m_skyTexId = p_skyTexId;
    m_vertices = 0;
}

void SkyDome::Initialize()
{
    m_numberOfVertices = (int) ((360 / m_dTheta) * (90 / m_dPhi) * 4);

    if(m_vertices != 0)
        delete [] m_vertices;

    m_vertices = new Vertex[m_numberOfVertices];

    GenerateHemiShperePoints();
}

void SkyDome::GenerateHemiShperePoints()
{
    int i = 0;
    float vx, vy, vz, mag;
    for(int phi = 0 ; phi <= (90 - m_dPhi) ; phi += (int)m_dPhi)
    {
        for(int theta = 0 ; theta <= (360 - m_dTheta) ; theta += (int)m_dTheta)
        {
            m_vertices[i].x = m_radius * sinf(phi * DTOR) * cosf(theta * DTOR);
            m_vertices[i].y = m_radius * sinf(phi * DTOR) * sinf(theta * DTOR);
            m_vertices[i].z = m_radius * cosf(phi * DTOR);

            vx = m_vertices[i].x;
            vy = m_vertices[i].y;
            vz = m_vertices[i].z;

            mag = (float)sqrtf((vx * vx) + (vy * vy) + (vz * vz));
            vx /= mag;
            vy /= mag;
            vz /= mag;

            m_vertices[i].u = (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
            m_vertices[i].v = (float)(asin(vy) / PI) + 0.5f;

            i++;

            m_vertices[i].x = m_radius * sinf((phi + m_dPhi) * DTOR) * cosf(theta * DTOR);
            m_vertices[i].y = m_radius * sinf((phi + m_dPhi) * DTOR) * sinf(theta * DTOR);
            m_vertices[i].z = m_radius * cosf((phi + m_dPhi) * DTOR);

            vx = m_vertices[i].x;
            vy = m_vertices[i].y;
            vz = m_vertices[i].z;

            mag = (float)sqrtf((vx * vx) + (vy * vy) + (vz * vz));
            vx /= mag;
            vy /= mag;
            vz /= mag;

            m_vertices[i].u = (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
            m_vertices[i].v = (float)(asin(vy) / PI) + 0.5f;

            i++;

            m_vertices[i].x = m_radius * sinf(phi * DTOR) * cosf((theta + m_dTheta) * DTOR);
            m_vertices[i].y = m_radius * sinf(phi * DTOR) * sinf((theta + m_dTheta) * DTOR);
            m_vertices[i].z = m_radius * cosf(phi * DTOR);

            vx = m_vertices[i].x;
            vy = m_vertices[i].y;
            vz = m_vertices[i].z;

            mag = (float)sqrtf((vx * vx) + (vy * vy) + (vz * vz));
            vx /= mag;
            vy /= mag;
            vz /= mag;

            m_vertices[i].u = (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
            m_vertices[i].v = (float)(asin(vy) / PI) + 0.5f;

            i++;

            if(phi > -90 && phi < 90)
            {
                m_vertices[i].x = m_radius * sinf((phi + m_dPhi) * DTOR) * cosf((theta + m_dTheta) * DTOR);
                m_vertices[i].y = m_radius * sinf((phi + m_dPhi) * DTOR) * sinf((theta + m_dTheta) *DTOR);
                m_vertices[i].z = m_radius * cosf((phi + m_dPhi) * DTOR);

                vx = m_vertices[i].x;
                vy = m_vertices[i].y;
                vz = m_vertices[i].z;

                mag = (float)sqrtf((vx * vx) + (vy * vy) + (vz * vz));
                vx /= mag;
                vy /= mag;
                vz /= mag;

                m_vertices[i].u = (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
                m_vertices[i].v = (float)(asin(vy) / PI) + 0.5f;
                i++;
            }
        }
    }

    FixSeamProblem();
}

void SkyDome::FixSeamProblem()
{
    // Fix the problem at the seam
    for (int i=0; i < m_numberOfVertices - 3; i++)
    {
        if (m_vertices[i].u - m_vertices[i+1].u > 0.9f)
            m_vertices[i+1].u += 1.0f;

        if (m_vertices[i+1].u - m_vertices[i].u > 0.9f)
            m_vertices[i].u += 1.0f;

        if (m_vertices[i].u - m_vertices[i+2].u > 0.9f)
            m_vertices[i+2].u += 1.0f;

        if (m_vertices[i+2].u - m_vertices[i].u > 0.9f)
            m_vertices[i].u += 1.0f;

        if (m_vertices[i+1].u - m_vertices[i+2].u > 0.9f)
            m_vertices[i+2].u += 1.0f;

        if (m_vertices[i+2].u - m_vertices[i+1].u > 0.9f)
            m_vertices[i+1].u += 1.0f;

        if (m_vertices[i].v - m_vertices[i+1].v > 0.8f)
            m_vertices[i+1].v += 1.0f;

        if (m_vertices[i+1].v - m_vertices[i].v > 0.8f)
            m_vertices[i].v += 1.0f;

        if (m_vertices[i].v - m_vertices[i+2].v > 0.8f)
            m_vertices[i+2].v += 1.0f;

        if (m_vertices[i+2].v - m_vertices[i].v > 0.8f)
            m_vertices[i].v += 1.0f;

        if (m_vertices[i+1].v - m_vertices[i+2].v > 0.8f)
            m_vertices[i+2].v += 1.0f;

        if (m_vertices[i+2].v - m_vertices[i+1].v > 0.8f)
            m_vertices[i+1].v += 1.0f;
    }
}

void SkyDome::Draw(unsigned p_elapsedTime)
{
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_skyTexId);
    glPushMatrix();
    {
        glTranslatef(0.0f, -300.0f, 0.0f);
        glRotatef(timeGetTime()/2000.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(270, 1, 0, 0);
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0 ; i < m_numberOfVertices ; i++)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glTexCoord2f(m_vertices[i].u, m_vertices[i].v);
            glVertex3f(m_vertices[i].x, m_vertices[i].y, m_vertices[i].z);
        }
        glEnd();
    }
    glPopMatrix();
}