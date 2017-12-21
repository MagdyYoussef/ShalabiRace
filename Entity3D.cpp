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

#include "Entity3D.h"
#include <gl/glut.h>

Entity3D::Entity3D()
{
    m_worldMatrix.identity();
    m_orientation.identity();
    m_rotation.identity();
    
    m_right.set(1.0f, 0.0f, 0.0f);
    m_up.set(0.0f, 1.0f, 0.0f);
    m_forward.set(0.0f, 0.0f, -1.0f);
    
    m_position.set(0.0f, 0.0f, 0.0f);
    m_velocity.set(0.0f, 0.0f, 0.0f);
    m_eulerOrient.set(0.0f, 0.0f, 0.0f);
    m_eulerRotate.set(0.0f, 0.0f, 0.0f);
    
    m_constrainedToWorldYAxis = false;
}

Entity3D::~Entity3D()
{
}

void Entity3D::constrainToWorldYAxis(bool constrain)
{
    // Constraining rotations to the world Y axis means that all heading
    // changes are applied to the world Y axis rather than the entity's
    // local Y axis.

    m_constrainedToWorldYAxis = constrain;
}

void Entity3D::orient(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // orient() changes the direction the entity is facing. This directly
    // affects the orientation of the entity's right, up, and forward vectors.
    // orient() is usually called in response to the user's input if the entity
    // is able to be moved by the user.

    m_eulerOrient.x += pitchDegrees;
    m_eulerOrient.y += headingDegrees;
    m_eulerOrient.z += rollDegrees;

    if (m_eulerOrient.x > 360.0f)
        m_eulerOrient.x -= 360.0f;

    if (m_eulerOrient.x < -360.0f)
        m_eulerOrient.x += 360.0f;

    if (m_eulerOrient.y > 360.0f)
        m_eulerOrient.y -= 360.0f;

    if (m_eulerOrient.y < -360.0f)
        m_eulerOrient.y += 360.0f;

    if (m_eulerOrient.z > 360.0f)
        m_eulerOrient.z -= 360.0f;

    if (m_eulerOrient.z < -360.0f)
        m_eulerOrient.z += 360.0f;
}

void Entity3D::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // rotate() does not change the direction the entity is facing. This method
    // allows the entity to freely spin around without affecting its orientation
    // and its right, up, and forward vectors. For example, if this entity is
    // a planet, then rotate() is used to spin the planet on its y axis. If this
    // entity is an asteroid, then rotate() is used to tumble the asteroid as
    // it moves in space.

    m_eulerRotate.x += pitchDegrees;
    m_eulerRotate.y += headingDegrees;
    m_eulerRotate.z += rollDegrees;

    if (m_eulerRotate.x > 360.0f)
        m_eulerRotate.x -= 360.0f;

    if (m_eulerRotate.x < -360.0f)
        m_eulerRotate.x += 360.0f;

    if (m_eulerRotate.y > 360.0f)
        m_eulerRotate.y -= 360.0f;

    if (m_eulerRotate.y < -360.0f)
        m_eulerRotate.y += 360.0f;

    if (m_eulerRotate.z > 360.0f)
        m_eulerRotate.z -= 360.0f;

    if (m_eulerRotate.z < -360.0f)
        m_eulerRotate.z += 360.0f;
}

const Vector3 &Entity3D::getForwardVector() const
{
    return m_forward;
}

const Vector3 &Entity3D::getPosition() const
{
    return m_position;
}

const Vector3 &Entity3D::getRightVector() const
{
    return m_right;
}

const Vector3 &Entity3D::getUpVector() const
{
    return m_up;
}

const Vector3 &Entity3D::getVelocity() const
{
    return m_velocity;
}

const Matrix4 &Entity3D::getWorldMatrix() const
{
    return m_worldMatrix;
}

const AABB &Entity3D::getAABB() const
{
    return m_aabb;
}


void Entity3D::setPosition(float x, float y, float z)
{
    m_position.set(x, y, z);
}

void Entity3D::setVelocity(float x, float y, float z)
{
    m_velocity.set(x, y, z);
}

void Entity3D::setWorldMatrix(const Matrix4 &worldMatrix)
{
    m_worldMatrix = worldMatrix;
    m_orientation.fromMatrix(worldMatrix);
    m_position.set(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
    extractAxes();
}

void Entity3D::Update(unsigned p_elapsedTime)
{
    float elapsedTimeSec = (float)p_elapsedTime / 1000.0f;

    Vector3 velocityElapsed, eulerOrientElapsed, eulerRotateElapsed;
    Vector3 oldPos, heading;
    Quaternion temp;

    velocityElapsed = m_velocity * elapsedTimeSec;
    eulerOrientElapsed = m_eulerOrient * elapsedTimeSec;
    eulerRotateElapsed = m_eulerRotate * elapsedTimeSec;

    // Update the entity's position.
    
    extractAxes();

    oldPos = m_position;

    m_position += m_right * velocityElapsed.x;
    m_position += m_up * velocityElapsed.y;
    m_position += m_forward * velocityElapsed.z;

    heading = m_position - oldPos;
    heading.normalize();

    // Update the entity's orientation.
    
    temp = eulerToQuaternion(m_orientation.toMatrix4(), eulerOrientElapsed.y,
            eulerOrientElapsed.x, eulerOrientElapsed.z);
    
    // When moving backwards invert rotations to match direction of travel.
    if (Vector3::dot(heading, m_forward) < 0.0f)
        temp = temp.inverse();

    m_orientation *= temp;
    m_orientation.normalize();

    // Update the entity's free rotation.

    temp = eulerToQuaternion(m_rotation.toMatrix4(), eulerRotateElapsed.y,
            eulerRotateElapsed.x, eulerRotateElapsed.z);
    
    m_rotation *= temp;
    m_rotation.normalize();

    // Update the entity's world matrix.

    temp = m_rotation * m_orientation;
    temp.normalize();

    m_worldMatrix = temp.toMatrix4();
    m_worldMatrix[3][0] = m_position.x;
    m_worldMatrix[3][1] = m_position.y;
    m_worldMatrix[3][2] = m_position.z;

    // Clear the entity's cached euler rotations and velocity for this frame.

    //m_velocity.set(0.0f, 0.0f, 0.0f);
    m_eulerOrient.set(0.0f, 0.0f, 0.0f);
    m_eulerRotate.set(0.0f, 0.0f, 0.0f);

    UpdateAABB();

}

Quaternion Entity3D::eulerToQuaternion(const Matrix4 &m, float headingDegrees,
                                       float pitchDegrees, float rollDegrees) const
{
    // Construct a quaternion from an euler transformation. We do this rather
    // than use Quaternion::fromHeadPitchRoll() to support constraining heading
    // changes to the world Y axis.

    Quaternion result = Quaternion::IDENTITY;
    Quaternion rotation = Quaternion::IDENTITY;
    Vector3 localXAxis(m[0][0], m[0][1], m[0][2]);
    Vector3 localYAxis(m[1][0], m[1][1], m[1][2]);
    Vector3 localZAxis(m[2][0], m[2][1], m[2][2]);   

    if (headingDegrees != 0.0f)
    {
        if (m_constrainedToWorldYAxis)
            rotation.fromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), headingDegrees);
        else
            rotation.fromAxisAngle(localYAxis, headingDegrees);

        result *= rotation;
    }

    if (pitchDegrees != 0.0f)
    {
        rotation.fromAxisAngle(localXAxis, pitchDegrees);
        result *= rotation;
    }

    if (rollDegrees != 0.0f)
    {
        rotation.fromAxisAngle(localZAxis, rollDegrees);
        result *= rotation;
    }

    return result;
}

void Entity3D::extractAxes()
{
    Matrix4 m = m_orientation.toMatrix4();

    m_right.set(m[0][0], m[0][1], m[0][2]);
    m_right.normalize();
    
    m_up.set(m[1][0], m[1][1], m[1][2]);
    m_up.normalize();
    
    m_forward.set(-m[2][0], -m[2][1], -m[2][2]);
    m_forward.normalize();
}

void Entity3D::Draw(unsigned p_elapsedTime)
{
#ifdef DEBUG_AABB
    DrawAABB();
#endif
}

void Entity3D::DrawAABB()
{
    float width = m_aabb.Max.x - m_aabb.Min.x;
    float height = m_aabb.Max.y - m_aabb.Min.y;
    float depth = m_aabb.Max.z - m_aabb.Min.z;

    glColor3f(1, 0, 0);
    glPushMatrix();
    {
 /*       glTranslatef(
            (m_aabb.Max.x + m_aabb.Min.x) / 2.0f + m_position.x,
            (m_aabb.Max.y + m_aabb.Min.y) / 2.0f + m_position.y,
            (m_aabb.Max.z + m_aabb.Min.z) / 2.0f + m_position.z);*/

        glTranslatef(
            (m_aabb.Max.x + m_aabb.Min.x) / 2.0f,
            (m_aabb.Max.y + m_aabb.Min.y) / 2.0f,
            (m_aabb.Max.z + m_aabb.Min.z) / 2.0f);

        glScalef(width, height, depth);
        glutWireCube(1.0f);
    }
    glPopMatrix();
}

void Entity3D::UpdateAABB()
{
	Vector3 *vertices = 0;
	int size;
	vertices = GetVertices(size);
	Vector3 minimum, maximum;
	
    for(Vector3* vertex = vertices; vertex < vertices + size; vertex++)
    {
        *vertex = *vertex * getWorldMatrix();
    }

    Math::SetMinMaxBound(vertices, size, minimum, maximum);

    //minimum = minimum * getWorldMatrix();
    //maximum = maximum * getWorldMatrix();
    maximum = maximum + m_position;
    minimum = minimum + m_position;

	m_aabb.Max = maximum;
	m_aabb.Min = minimum;
}