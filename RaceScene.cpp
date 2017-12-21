#include "RaceScene.h"
#include "ShalabiRace.h"
#include <gl/glut.h>
#include "TextureLoader.h"
#include "Car.h"
#include <iostream>
#include <sstream>
#include "Noobs.h"
#include <time.h>
using namespace std;
RaceScene::RaceScene(void)
{
    m_numberOfNoobs = NUMBER_OF_NOOBS;
    m_noobs.resize(NUMBER_OF_NOOBS);
	srand(time(NULL));
}

RaceScene::~RaceScene(void)
{

}

void RaceScene::GetNewNoob(Noob* &p_noob)
{
	int noobIndex = 0;

	p_noob = new Noob(m_terrain, const_cast<char*>(NoobModels[noobIndex].m_modelFileName.c_str()),
		const_cast<char*>(NoobModels[noobIndex].m_texFileName.c_str()));
}

void RaceScene::Initialize()
{
     if (!m_font.create("Arial", 20, GLFont::BOLD))
        throw std::runtime_error("Failed to create font.");

    m_camera.enableSpringSystem(true);
    m_camera.perspective(
        CAMERA_FOVX,
        static_cast<float>(ShalabiRace::WindowWidth) / static_cast<float>(ShalabiRace::WindowHeight),
        CAMERA_ZNEAR,
        CAMERA_ZFAR);


    m_components.clear();

    srand(timeGetTime());

    unsigned skyTexId = LoadTexture("Textures\\skydome.bmp");
    m_sky = new SkyDome(8192, 8, 8, skyTexId);
    m_components.push_back(m_sky);

    m_terrain = new Terrain(
        "Textures\\hill.bmp",
        "Textures\\hill512.raw",
        512,
        512,
        32,
        2.0f);

    m_components.push_back(m_terrain);

    m_noob = new BaseCar(0, m_terrain);
    m_noob->constrainToWorldYAxis(true);
    m_components.push_back(m_noob);

    m_player = new Car(&m_camera, m_terrain);
    m_player->constrainToWorldYAxis(true);
    m_components.push_back(m_player);

    for(int i = 0 ; i < NUMBER_OF_NOOBS ; i++)
	{
		Noob *tempNoob;
		GetNewNoob(tempNoob);
		tempNoob->setPosition((rand() % 3500) * ((i % 2? -1 : 1)), 100, rand() % 1000);
		tempNoob->orient(rand() % 180, 0, 0);
		m_noobs[i] = tempNoob;
		m_components.push_back(tempNoob);
	}

    Scene::Initialize();
}

void RaceScene::LoadContent()
{
}

void RaceScene::Update(unsigned p_elapsedTime)
{
    Scene::Update(p_elapsedTime);

    if(AABB::Collide(m_player->getAABB(), m_noob->getAABB()))
    {

        m_camera.ShakeWorld();
        Vector3 v = m_player->getForwardVector() - m_noob->getForwardVector();
        
        Vector3 &pos = const_cast<Vector3&>(m_noob->getPosition());
        pos.addScaledVector(v, 50);

        m_player->setVelocity(0, 0, -100);
    }

	for(int i = 0 ; i < NUMBER_OF_NOOBS ; i++)
	{
		if(AABB::Collide(m_player->getAABB(), m_noobs[i]->getAABB()))
		{
			//AABB::Collide(m_player->getAABB(), m_noobs[i]->getAABB());
			m_noobs[i]->SetAnimation(DEATH_FALLBACK);
            m_numberOfNoobs--;
		}
		else
		{
			//cout << endl;
		}

		if(m_noobs[i]->m_dieTime > 3000)
		{
			m_noobs[i]->Visible(false);
		}
	}
    m_camera.update((float)p_elapsedTime / 1000.0f);
}

void RaceScene::Draw(unsigned p_elapsedTime)
{
    DrawHUD();

    //glwrite
    m_camera.SetTransformations();
    glPushMatrix();
    {
        Scene::Draw(p_elapsedTime);
    }
    glPopMatrix();

}

void RaceScene::DrawHUD()
{
    ostringstream output;
    output.clear();
    output << "HELLO WORLD loooooooooooooo \n HAHAHAH !!!!";

    m_font.begin();
    m_font.setColor(1.0f, 0.0f, 0.0f);
    m_font.drawText(50, 50, output.str().c_str());
    m_font.end();
}


