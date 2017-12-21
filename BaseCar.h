#ifndef BASE_CAR
#define BASE_CAR
#include "Entity3D.h"
#include "ThirdPersonCamera.h"
#include "3DModel.h"
#include "Terrain.h"

// FLE
const float DefaultReverseAcceleration = 100.0f;
const float DefaultBreakingDeacceleration = 800.0f;
const float DefaultForwardAcceleration = 180.0f;
const float DefaultDeacceleration = 240.0f;
const float DefaultTopSpeed = 1400.0f;
const float DefaultMaximumSteeringAngle =  45.0f * Math::PI / 180.0f; // Radians


enum EngineState
{
    Accelerating,
    Deaccelerating,
    Braking,
    Stopped
};

enum Gear
{
    Gear0,
    GearReverse
};

class BaseCar : public Entity3D
{
protected:
    ThirdPersonCamera *m_camera;
    Terrain* m_terrain;

    t3DModel* m_model;

    EngineState m_currentState;
    Gear m_currentGear;
    float m_forwardAcceleration;
    float m_reverseAcceleration;
    float m_currentSteeringRadius;
    float m_currentSteeringAngle;
    Vector3* m_vertices;
    int m_numberOfVertices;
public:
    ThirdPersonCamera* Camera(); 
    void Camera(ThirdPersonCamera* p_camera); 
    EngineState State();

    BaseCar(ThirdPersonCamera* p_camera, Terrain* p_terrain);

    BaseCar(void);
    ~BaseCar();

    void Initialize();
    void Update(unsigned p_elapsedTime);
    void Draw(unsigned p_elapsedTime);
    void SteerRight();
    void SteerLeft();
    void Accelerate();
    void Brake();
    void Deaccelerate();
    void ResetSteering();
    void Reverse();
    void PrintState(EngineState m_state, Gear m_gear);
    void UpdateIncline();
protected:
	Vector3*  GetVertices(int &size);
    void ApplySteering(float p_velocitySignedMagnitude, float p_elapsedTimeSeconds);
    void Steer(float p_angle);
    float GetEngineAcceleration();
    void UpdateCarPhysics(unsigned p_elapsedTime);

};

#endif