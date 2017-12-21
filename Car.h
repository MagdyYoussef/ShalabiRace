#ifndef CAR_H
#define CAR_H
#include "Entity3D.h"
#include "ThirdPersonCamera.h"
#include "3DModel.h"
#include "BaseCar.h"

class Car : public BaseCar
{
public:
    Car(void);
    Car(ThirdPersonCamera* p_camera, Terrain* p_terrain);
    ~Car();
    void Initialize();
    void Draw(unsigned p_elapsedTime);
    void Update(unsigned p_elapsedTime);
};

#endif