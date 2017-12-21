#include "ShalabiRace.h"
#include <gl/glew.h>
#include <gl/glut.h>
#include "ThirdPersonCamera.h"
#include "RaceScene.h"
#include "Keyboard.h"

ShalabiRace::ShalabiRace(void)
{
    
}

ShalabiRace::~ShalabiRace(void)
{
    delete m_currentScene;
}

void ShalabiRace::Initialize()
{
    m_currentScene = new RaceScene();
    m_currentScene->Initialize();
}

void ShalabiRace::LoadContent()
{
    m_currentScene->LoadContent();
}

void ShalabiRace::Update(unsigned p_elapsedTime)
{
    Keyboard &keyboard = Keyboard::Instance();

    if(keyboard.KeyDown(Keyboard::KEY_ESCAPE))
        exit(0);

    if(keyboard.KeyUp(keyboard.KEY_P))
    {
        m_currentScene->Enabled(!m_currentScene->Enabled());
    }
    if(m_currentScene->Enabled())
    {
        m_currentScene->Update(p_elapsedTime);
    }


    
}

void ShalabiRace::Draw(unsigned p_elapsedTime)
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, ShalabiRace::WindowWidth, ShalabiRace::WindowHeight);

    m_currentScene->Draw(p_elapsedTime);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}
