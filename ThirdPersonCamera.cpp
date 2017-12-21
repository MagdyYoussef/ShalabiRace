//-----------------------------------------------------------------------------
// Copyright (c) 2008 dhpoware. All Rights Reserved.
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

#include <cmath>
#include <gl/glew.h>
#include <gl/glut.h>
#include "ThirdPersonCamera.h"

const float ThirdPersonCamera::DEFAULT_SPRING_CONSTANT = 16.0f;
const float ThirdPersonCamera::DEFAULT_DAMPING_CONSTANT = 8.0f;

const float ThirdPersonCamera::DEFAULT_FOVX = 80.0f;
const float ThirdPersonCamera::DEFAULT_ZFAR = 1000.0f;
const float ThirdPersonCamera::DEFAULT_ZNEAR = 1.0f;

const Vector3 ThirdPersonCamera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const Vector3 ThirdPersonCamera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const Vector3 ThirdPersonCamera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

ThirdPersonCamera::ThirdPersonCamera()
{
    m_enableSpringSystem = true;
    m_isShaking = false;
    m_springConstant = DEFAULT_SPRING_CONSTANT;
    m_dampingConstant = DEFAULT_DAMPING_CONSTANT;

    m_offsetDistance = 0.0f;
    m_headingDegrees = 0.0f;
    m_pitchDegrees = 0.0f;

    m_fovx = DEFAULT_FOVX;
    m_znear = DEFAULT_ZNEAR;
    m_zfar = DEFAULT_ZFAR;

    m_eye.set(0.0f, 0.0f, 0.0f);
    m_target.set(0.0f, 0.0f, 0.0f);
    m_targetYAxis.set(0.0f, 1.0f, 0.0f);

    m_xAxis.set(1.0f, 0.0f, 0.0f);
    m_yAxis.set(0.0f, 1.0f, 0.0f);
    m_zAxis.set(0.0f, 0.0f, 1.0f);
    m_viewDir.set(0.0f, 0.0f, -1.0f);

    m_velocity.set(0.0f, 0.0f, 0.0f);

    m_viewMatrix.identity();
    m_projMatrix.identity();
    m_orientation.identity();
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::enableSpringSystem(bool enableSpringSystem)
{
    m_enableSpringSystem = enableSpringSystem;
}

void ThirdPersonCamera::SetTransformations()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&getProjectionMatrix()[0][0]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&getViewMatrix()[0][0]);
}

void ThirdPersonCamera::lookAt(const Vector3 &target)
{
    m_target = target;
}

void ThirdPersonCamera::lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up)
{
    m_eye = eye;
    m_target = target;
    m_targetYAxis = up;

    m_zAxis = eye - target;
    m_zAxis.normalize();

    m_viewDir = -m_zAxis;

    m_xAxis = Vector3::cross(up, m_zAxis);
    m_xAxis.normalize();

    m_yAxis = Vector3::cross(m_zAxis, m_xAxis);
    m_yAxis.normalize();
    m_xAxis.normalize();

    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[1][0] = m_xAxis.y;
    m_viewMatrix[2][0] = m_xAxis.z;
    m_viewMatrix[3][0] = -Vector3::dot(m_xAxis, eye);

    m_viewMatrix[0][1] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[2][1] = m_yAxis.z;
    m_viewMatrix[3][1] = -Vector3::dot(m_yAxis, eye);

    m_viewMatrix[0][2] = m_zAxis.x;
    m_viewMatrix[1][2] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;    
    m_viewMatrix[3][2] = -Vector3::dot(m_zAxis, eye);

    m_orientation.fromMatrix(m_viewMatrix);

    Vector3 offset = m_target - m_eye;

    m_offsetDistance = offset.magnitude();
}

void ThirdPersonCamera::perspective(float fovx, float aspect, float znear, float zfar)
{
    float e = 1.0f / tanf(Math::degreesToRadians(fovx) / 2.0f);
    float aspectInv = 1.0f / aspect;
    float fovy = 2.0f * atanf(aspectInv / e);
    float xScale = 1.0f / tanf(0.5f * fovy);
    float yScale = xScale / aspectInv;

    m_projMatrix[0][0] = xScale;
    m_projMatrix[0][1] = 0.0f;
    m_projMatrix[0][2] = 0.0f;
    m_projMatrix[0][3] = 0.0f;

    m_projMatrix[1][0] = 0.0f;
    m_projMatrix[1][1] = yScale;
    m_projMatrix[1][2] = 0.0f;
    m_projMatrix[1][3] = 0.0f;

    m_projMatrix[2][0] = 0.0f;
    m_projMatrix[2][1] = 0.0f;
    m_projMatrix[2][2] = (zfar + znear) / (znear - zfar);
    m_projMatrix[2][3] = -1.0f;

    m_projMatrix[3][0] = 0.0f;
    m_projMatrix[3][1] = 0.0f;
    m_projMatrix[3][2] = (2.0f * zfar * znear) / (znear - zfar);
    m_projMatrix[3][3] = 0.0f;

    m_fovx = fovx;
    m_znear = znear;
    m_zfar = zfar;
}

void ThirdPersonCamera::rotate(float headingDegrees, float pitchDegrees)
{
    m_headingDegrees = -headingDegrees;
    m_pitchDegrees = -pitchDegrees;
}

void ThirdPersonCamera::setOffsetDistance(float offsetDistance)
{
    m_offsetDistance = offsetDistance;
}

void ThirdPersonCamera::setSpringConstant(float springConstant)
{
    // We're using a critically damped spring system where the damping ratio
    // is equal to one.
    //
    // damping ratio = m_dampingConstant / (2.0f * sqrtf(m_springConstant))

    m_springConstant = springConstant;
    m_dampingConstant = 2.0f * sqrtf(springConstant);
}

void ThirdPersonCamera::update(float elapsedTimeSec)
{
    if(m_isShaking)
    {
        m_shakeTime += elapsedTimeSec;
        if(m_shakeTime > 5.0f)
        {
            m_shakeTime = 0;
            m_isShaking = false;
            m_springConstant = ThirdPersonCamera::DEFAULT_SPRING_CONSTANT;
        }
    }

    updateOrientation(elapsedTimeSec);

    if (m_enableSpringSystem)
        updateViewMatrix(elapsedTimeSec);
    else
        updateViewMatrix();
}


void ThirdPersonCamera::ShakeWorld()
{
    if(!m_isShaking)
    {
        m_isShaking = true;
        m_springConstant = 500;
        m_shakeTime = 0;
    }
}

void ThirdPersonCamera::updateOrientation(float elapsedTimeSec)
{
    m_pitchDegrees *= elapsedTimeSec;
    m_headingDegrees *= elapsedTimeSec;

    Quaternion rot;

    if (m_headingDegrees != 0.0f)
    {
        rot.fromAxisAngle(m_targetYAxis, m_headingDegrees);
        m_orientation = rot * m_orientation;
    }

    if (m_pitchDegrees != 0.0f)
    {
        rot.fromAxisAngle(WORLD_XAXIS, m_pitchDegrees);
        m_orientation = m_orientation * rot;
    }
}

void ThirdPersonCamera::updateViewMatrix()
{
    m_viewMatrix = m_orientation.toMatrix4();

    m_xAxis.set(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis.set(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis.set(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);
    m_viewDir = -m_zAxis;

    m_eye = m_target + m_zAxis * m_offsetDistance;

    m_viewMatrix[3][0] = -Vector3::dot(m_xAxis, m_eye);
    m_viewMatrix[3][1] = -Vector3::dot(m_yAxis, m_eye);
    m_viewMatrix[3][2] = -Vector3::dot(m_zAxis, m_eye);
}

void ThirdPersonCamera::updateViewMatrix(float elapsedTimeSec)
{
    m_viewMatrix = m_orientation.toMatrix4();

    m_xAxis.set(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis.set(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis.set(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);

    // Calculate the new camera position. The 'idealPosition' is where the
    // camera should be position. The camera should be positioned directly
    // behind the target at the required offset distance. What we're doing here
    // is rather than have the camera immediately snap to the 'idealPosition'
    // we slowly move the camera towards the 'idealPosition' using a spring
    // system.
    //
    // References:
    //   Stone, Jonathan, "Third-Person Camera Navigation," Game Programming
    //     Gems 4, Andrew Kirmse, Editor, Charles River Media, Inc., 2004.

    Vector3 idealPosition = m_target + m_zAxis * m_offsetDistance;
    Vector3 displacement = m_eye - idealPosition;
    Vector3 springAcceleration = (-m_springConstant * displacement) - 
        (m_dampingConstant * m_velocity);

    m_velocity += springAcceleration * elapsedTimeSec;
    m_eye += m_velocity * elapsedTimeSec;

    // The view matrix is always relative to the camera's current position
    // 'm_eye'. Since a spring system is being used here 'm_eye' will be
    // relative to 'idealPosition'. When the camera is no longer being
    // moved 'm_eye' will become the same as 'idealPosition'. The local
    // x, y, and z axes that were extracted from the camera's orientation
    // 'm_orienation' is correct for the 'idealPosition' only. We need
    // to recompute these axes so that they're relative to 'm_eye'. Once
    // that's done we can use those axes to reconstruct the view matrix.

    m_zAxis = m_eye - m_target;
    m_zAxis.normalize();

    m_xAxis = Vector3::cross(m_targetYAxis, m_zAxis);
    m_xAxis.normalize();

    m_yAxis = Vector3::cross(m_zAxis, m_xAxis);
    m_yAxis.normalize();

    m_viewMatrix.identity();

    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[1][0] = m_xAxis.y;
    m_viewMatrix[2][0] = m_xAxis.z;
    m_viewMatrix[3][0] = -Vector3::dot(m_xAxis, m_eye);

    m_viewMatrix[0][1] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[2][1] = m_yAxis.z;
    m_viewMatrix[3][1] = -Vector3::dot(m_yAxis, m_eye);

    m_viewMatrix[0][2] = m_zAxis.x;
    m_viewMatrix[1][2] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;   
    m_viewMatrix[3][2] = -Vector3::dot(m_zAxis, m_eye);

    m_viewDir = -m_zAxis;
}


