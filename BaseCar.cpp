#include "BaseCar.h"
#include <gl/glut.h>
#include "keyboard.h"
#include "float.h"
#include <windows.h>
#include <iostream>
using namespace std;

ThirdPersonCamera* BaseCar::Camera()
{
    return m_camera;
}

void BaseCar::Camera(ThirdPersonCamera* p_camera)
{
    m_camera = p_camera;
}



EngineState BaseCar::State()
{
    return m_currentState;
}

BaseCar::BaseCar(void)
{
    m_camera = 0;
    m_terrain = 0;
    m_numberOfVertices = -1;
}

BaseCar::BaseCar(ThirdPersonCamera* p_camera, Terrain* p_terrain)
{
    m_camera = p_camera;
    m_terrain = p_terrain;
    m_numberOfVertices = -1;
}

BaseCar::~BaseCar()
{
    delete m_model;
}

void BaseCar::Initialize()
{
    m_model = new t3DModel();
    m_model->LoadMD2Model("Models\\hummer.md2", "Models\\hummer.bmp");

    m_numberOfVertices = 0;
	for(int i = 0 ; i < m_model->numOfObjects ; i++)
	{
		m_numberOfVertices += m_model->pObjects[i].numOfVerts;
	}

	m_vertices = new Vector3[m_numberOfVertices];
	int count = 0;

	for(int i = 0 ; i < m_model->numOfObjects ; i++)
	{
		for(int j = 0 ; j < m_model->pObjects[i].numOfVerts ; j++)
		{
			m_vertices[count++] = m_model->pObjects[i].pVerts[j];
		}
	}

	UpdateAABB();
    m_currentState = Stopped;
    m_currentGear = Gear0;
	
    m_currentSteeringAngle = 0;
    m_currentSteeringRadius = 0;
    constrainToWorldYAxis(true);
    
    setPosition(0, 0, 500);

    if(m_camera != 0)
    {
        m_camera->lookAt(
            Vector3(0.0f, 180.0f, 270.0f),
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 1.0f, 0.0f));
    }

}

void BaseCar::Update(unsigned p_elapsedTime)
{
    UpdateCarPhysics(p_elapsedTime);
    Entity3D::Update(p_elapsedTime);
	//UpdateAABB();
    
    if(m_terrain != 0)
    {
        float height = m_terrain->GetHeightmapEntryFromCoord(m_position.x, m_position.z);
        m_position.set(m_position.x, height, m_position.z);

    }
    if(m_camera != 0)
    {
        m_camera->lookAt(getPosition());
    }
}

void BaseCar::PrintState(EngineState m_state, Gear m_gear)
{
    char title[256];
    char* state;
    char* gear;

    switch(m_state)
    {
    case 0:
        state = "Accelerating";
        break;
    case 1:
        state = "Deaccelerating";
        break;
    case 2:
        state = "Braking";
        break;
    case 3:
        state = "Stopped";
        break;
    }

    switch(m_gear)
    {
    case 0:
        gear = "Gear0";
        break;
    case 1:
        gear = "Reverse";
        break;
    }
    
    sprintf(title, "%s %s", state, gear);
    glutSetWindowTitle(title);
}

void BaseCar::UpdateCarPhysics(unsigned p_elapsedTime)
{
    PrintState(m_currentState, m_currentGear);
    float acceleration = GetEngineAcceleration();
    float timeSeconds = p_elapsedTime / 1000.0f;

    Vector3 newVelocity = m_velocity;
    Vector3 velocityIncrease(0, 0, acceleration);
    newVelocity.addScaledVector(velocityIncrease, timeSeconds);

    if(newVelocity.magnitude() <= DefaultTopSpeed)
    {
        m_velocity = newVelocity;
    }

    if((m_currentGear == Gear0 && newVelocity.z <= 0) ||
        (m_currentGear == GearReverse && newVelocity.z >= 0))
    //if(newVelocity.z <= 0)
    {
        //cout << newVelocity.z << endl;
        m_currentState = Stopped;

        m_velocity.reset();
    }
    
    /*if(Math::InRage(m_velocity.magnitude(), 0.5f))
    {
        m_currentState = Stopped;
        m_velocity.reset();
    }*/

    ApplySteering(m_velocity.z, p_elapsedTime / 1000.0f);
}

/*
 * According to ackerman's steering theorem, a car with wheel
 * base L (the distance between the front and rear wheels) and
 * front wheel angle Theta, will steer in a circle of radius:
 * currentSteeringRadius = L / Sin(Theta) -- (1)
 * and will have angular velocity (omega) = Linear Velocity / currentSteeringRadius -- (2)
 * from (1) and (2) the final angular velocity of the car will be:
 * omega = (Linear_Velocity * Sin(Theta)) ? / L
 */
void BaseCar::ApplySteering(float p_velocitySignedMagnitude, float p_elapsedTimeSeconds)
{
    if (m_currentSteeringAngle < 0)   // Steering Right
        m_currentSteeringRadius = -10 / abs(sin(m_currentSteeringAngle));
    else if (m_currentSteeringAngle > 0) // Steering Left
        m_currentSteeringRadius = 10 / abs(sin(m_currentSteeringAngle));
    else
        m_currentSteeringRadius = FLT_MAX ; // for forward movement, an infinite radius is used


    // update the car's orientation according to the steering radius
    float orientationDegrees = Math::radiansToDegrees(abs(p_velocitySignedMagnitude) * p_elapsedTimeSeconds / m_currentSteeringRadius);

    orient(orientationDegrees, 0.0f, 0.0f);

    if(p_velocitySignedMagnitude < 0)
        orientationDegrees *= -1;

    if(m_camera != 0)
    {
        m_camera->rotate(orientationDegrees, 0.0f);
    }
}

float BaseCar::GetEngineAcceleration()
{
    if(m_currentState == Accelerating)
    {
        if(m_currentGear == Gear0)
            return DefaultForwardAcceleration;
        else if(m_currentGear == GearReverse)
            return -DefaultReverseAcceleration;
    }

    else if(m_currentState == Deaccelerating)
    {
        if(m_currentGear == Gear0)
            return -DefaultDeacceleration;
        else if(m_currentGear == GearReverse)
            return DefaultDeacceleration;
    }
    
    else if(m_currentState == Braking)
    {
        if(m_currentGear == Gear0)
            return -DefaultBreakingDeacceleration;
        else if(m_currentGear == GearReverse)
            return DefaultBreakingDeacceleration;
    }

    else if(m_currentState == Stopped)
        return 0;

    return 0;
}

void BaseCar::Steer(float p_angle)
{
    if (abs(m_currentSteeringAngle) < DefaultMaximumSteeringAngle)
    {
        m_currentSteeringAngle += p_angle;
    }
}

void BaseCar::SteerRight()
{
    Steer(-0.06f);
}

void BaseCar::SteerLeft()
{
    Steer(0.06f);
}

void BaseCar::ResetSteering()
{
    m_currentSteeringAngle = 0;
}

void BaseCar::Accelerate()
{
        PlaySound(NULL, NULL, NULL);
        PlaySound(TEXT("forward.wav"), NULL, SND_ASYNC | SND_LOOP);

    m_currentState = Accelerating;
    m_currentGear = Gear0;
}

void BaseCar::Brake()
{
    PlaySound(NULL, NULL, NULL);
    PlaySound(TEXT("brake.wav"), NULL, SND_ASYNC);
    m_currentState = Braking;
}

void BaseCar::Deaccelerate()
{
    PlaySound(NULL, NULL, NULL);
    PlaySound(TEXT("engineIdle_2.wav"), NULL, SND_ASYNC | SND_LOOP);
    m_currentState = Deaccelerating;
}

void BaseCar::Reverse()
{
        PlaySound(NULL, NULL, NULL);
        PlaySound(TEXT("reverse.wav"), NULL, SND_ASYNC | SND_LOOP);
    m_currentState = Accelerating;
    m_currentGear = GearReverse;
}

Vector3* BaseCar::GetVertices(int &size)
{
    if(m_numberOfVertices == -1)
    {
        m_numberOfVertices = 0;
        for(int i = 0 ; i < m_model->numOfObjects ; i++)
        {
            m_numberOfVertices += m_model->pObjects[i].numOfVerts;
        }

	    m_vertices = new Vector3[m_numberOfVertices];
    }
    

    int count = 0;

    for(int i = 0 ; i < m_model->numOfObjects ; i++)
    {
        for(int j = 0 ; j < m_model->pObjects[i].numOfVerts ; j++)
        {
            m_vertices[count++] = m_model->pObjects[i].pVerts[j];
        }
    }
    size = m_numberOfVertices;
	return m_vertices;
}

void BaseCar::Draw(unsigned p_elapsedTime)
{
    glPushMatrix();
    {
        glMultMatrixf(&getWorldMatrix()[0][0]);
        glPushMatrix();
        {
            m_model->RenderFrame(0);
        }
        glPopMatrix();

		//glColor3f(1, 0, 0);

        /*glBegin(GL_LINES);
        {
            glVertex3f(m_aabb.Max.x, m_aabb.Max.y, m_aabb.Max.z); 
            glVertex3f(m_aabb.Min.x, m_aabb.Min.y, m_aabb.Min.z);
        }
        glEnd();*/
        //glutWireSphere(20, 16, 16);
    }
    glPopMatrix();

    
    Entity3D::Draw(p_elapsedTime);
}

void BaseCar::UpdateIncline()
{
    Vector3 carHead = m_aabb.Max;
    Vector3 carTail = m_aabb.Min;

    carHead.z = m_aabb.Min.z;

    carHead.y = m_terrain->GetHeightmapEntryFromCoord(carHead.x, carHead.z);
    carTail.y = m_terrain->GetHeightmapEntryFromCoord(carTail.x, carTail.z);

    Vector3 incline = carTail - carHead;

    float cosTheta = Vector3::dot(m_forward, incline) / m_forward.magnitude() * incline.magnitude();
    float theta = acos(cosTheta);

}