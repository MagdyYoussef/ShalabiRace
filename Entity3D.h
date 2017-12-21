//-----------------------------------------------------------------------------
// Copyright (c) 2006 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#if !defined(ENTITY3D_3D_H)
#define ENTITY3D_3D_H

#define DEBUG_AABB

#include "mathlib.h"
#include "DrawableGameComponent.h"
//-----------------------------------------------------------------------------
// The Entity3D class tracks an object's position and orientation in 3D space.
//
// An entity can be rotated and oriented. By default all heading changes will
// be about the entity's local Y axis. The constrainToWorldYAxis() method
// forces all heading changes to be about the world Y axis rather that the
// entity's local Y axis.
//
// Changing the entity's orientation using the orient() method will update the
// direction the entity is facing. This will also update the entity's right,
// up, and forward vectors.
//
// Rotating an entity using the rotate() method will not update the direction
// the entity is facing. The rotate() method is provided as a means to animate
// the entity.
//
// For example, consider an Entity3D object that represents an asteroid. The
// rotate() method would be used to tumble the asteroid as it moves through
// space. The orient() method would be used to alter the direction the asteroid
// is moving.
//-----------------------------------------------------------------------------

class Entity3D : public DrawableGameComponent
{
public:
    Entity3D();
    ~Entity3D();
    
    void constrainToWorldYAxis(bool constrain);

    void orient(float headingDegrees, float pitchDegrees, float rollDegrees);
    void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);
   
    const Vector3 &getForwardVector() const;
    const Vector3 &getPosition() const;
    const Vector3 &getRightVector() const;
    const Vector3 &getUpVector() const;
    const Vector3 &getVelocity() const;
    const Matrix4 &getWorldMatrix() const;
    const AABB &getAABB() const;
    
    void setPosition(float x, float y, float z);
    void setVelocity(float x, float y, float z);
    void setWorldMatrix(const Matrix4 &worldMatrix);
    void Draw(unsigned p_elapsedTime);
    void Update(unsigned p_elapsedTime);
protected:
	void UpdateAABB();
    // FILI
    void DrawAABB();
	virtual Vector3* GetVertices(int& size) = 0;
private:
    Quaternion eulerToQuaternion(const Matrix4 &m, float headingDegrees,
                                 float pitchDegrees, float rollDegrees) const;
    void extractAxes();

    Matrix4 m_worldMatrix;
    Quaternion m_orientation;
    Quaternion m_rotation;
    
    Vector3 m_right;
    Vector3 m_up;
    Vector3 m_eulerOrient;
    Vector3 m_eulerRotate;
    bool m_constrainedToWorldYAxis;

protected:
    Vector3 m_forward;
    Vector3 m_position;
    Vector3 m_velocity;
	AABB m_aabb;
};

#endif