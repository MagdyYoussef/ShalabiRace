#include "Car.h"
#include <gl/glut.h>
#include "keyboard.h"
#include "float.h"
#include <iostream>
using namespace std;

Car::Car(void)
{
    m_camera = 0;
}

Car::Car(ThirdPersonCamera* p_camera, Terrain* p_terrain) 
: BaseCar(p_camera, p_terrain)
{}

Car::~Car()
{
    delete m_model;
}

void Car::Initialize()
{
    m_model = new t3DModel();
    m_model->LoadMD2Model("Models\\watertruck.md2", "Models\\watertruck.bmp");
	UpdateAABB();
    m_currentState = Stopped;
    m_currentGear = Gear0;
	
    m_currentSteeringAngle = 0;
    m_currentSteeringRadius = 0;
    constrainToWorldYAxis(true);
    
    setPosition(0, 0, 0);

    if(m_camera != 0)
    {
        m_camera->lookAt(
            Vector3(0.0f, 140.0f, 300.0f),
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 1.0f, 0.0f));
    }

}

void Car::Update(unsigned p_elapsedTime)
{
    Keyboard &keyboard = Keyboard::Instance();

    if (keyboard.KeyDown(Keyboard::KEY_UP))
    {
        if((m_currentState == Accelerating || m_currentState == Deaccelerating)&& m_currentGear == GearReverse)
        {
            
            Brake();
        }
        else if(m_currentState == Stopped || m_currentState == Deaccelerating)
        {
            Accelerate();
        }
    }

    if (keyboard.KeyUp(Keyboard::KEY_UP) || keyboard.KeyUp(Keyboard::KEY_DOWN))
    {
        
        Deaccelerate();
    }

    if (keyboard.KeyDown(Keyboard::KEY_DOWN))
    {
        if((m_currentState == Accelerating || m_currentState== Deaccelerating) && m_currentGear == Gear0)
        {
            Brake();
        }
        else if(m_currentState == Stopped || m_currentState == Deaccelerating)
        {
            Reverse();
        }
    }

    if (keyboard.KeyDown(Keyboard::KEY_RIGHT))
    {
        SteerRight();
    }

    if (keyboard.KeyDown(Keyboard::KEY_LEFT))
    {
        SteerLeft();
    }

    if(keyboard.KeyUp(Keyboard::KEY_LEFT) || keyboard.KeyUp(Keyboard::KEY_RIGHT))
    {
        ResetSteering();
    }

    BaseCar::Update(p_elapsedTime);
}


void Car::Draw(unsigned p_elapsedTime)
{
    

    glPushMatrix();
    {
        glMultMatrixf(&getWorldMatrix()[0][0]);
        glPushMatrix();
        {
            //glRotatef(180.0f, 0, 1, 0);
            m_model->RenderFrame(0);
        }
        glPopMatrix();
    }
    glPopMatrix();

    Entity3D::Draw(p_elapsedTime);
}