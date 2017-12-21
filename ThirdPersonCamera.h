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

#if !defined(THIRD_PERSON_CAMERA_H)
#define THIRD_PERSON_CAMERA_H

#include "mathlib.h"

//-----------------------------------------------------------------------------
// A quaternion based third person camera class. This camera model incorporates
// a spring system to smooth out camera movement. It is enabled by default but
// can be disabled.
//
// Call lookAt(eye, target, up) to establish the camera's initial settings.
// This will define the camera's initial position in relation to the target
// that the camera will be looking. When the target moves call the camera's
// lookAt(target) method to set the target's new world position. When the
// target rotates call the camera's rotate() method.
//
// The camera's update() method must be called once per frame. The update()
// method performs any pending camera rotations and recalculates the camera's
// view matrix.
//-----------------------------------------------------------------------------

class ThirdPersonCamera
{
public:
    ThirdPersonCamera();
    ~ThirdPersonCamera();

    void SetTransformations();
    void lookAt(const Vector3 &target);
    void lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);
    void perspective(float fovx, float aspect, float znear, float zfar);
    void rotate(float longitudeDegrees, float latitudeDegrees);
    void update(float elapsedTimeSec);
    void ShakeWorld();


    // Getter methods.

    float getDampingConstant() const;
    float getOffsetDistance() const;
    const Quaternion &getOrientation() const;
    const Vector3 &getPosition() const;
    const Matrix4 &getProjectionMatrix() const;
    float getSpringConstant() const;
    const Vector3 &getTargetYAxis() const;
    const Vector3 &getViewDirection() const;
    const Matrix4 &getViewMatrix() const;
    const Vector3 &getXAxis() const;
    const Vector3 &getYAxis() const;
    const Vector3 &getZAxis() const;
    bool springSystemIsEnabled() const;

    // Setter methods.

    void enableSpringSystem(bool enableSpringSystem);
    void setOffsetDistance(float offsetDistance);
    void setSpringConstant(float springConstant);

private:
    void updateOrientation(float elapsedTimeSec);
    void updateViewMatrix();
    void updateViewMatrix(float elapsedTimeSec);

    static const float DEFAULT_SPRING_CONSTANT;
    static const float DEFAULT_DAMPING_CONSTANT;
    static const float DEFAULT_FOVX;
    static const float DEFAULT_ZFAR;
    static const float DEFAULT_ZNEAR;
    static const Vector3 WORLD_XAXIS;
    static const Vector3 WORLD_YAXIS;
    static const Vector3 WORLD_ZAXIS;

    bool m_isShaking;
    float m_shakeTime;

    bool m_enableSpringSystem;
    float m_springConstant;
    float m_dampingConstant;
    float m_offsetDistance;
    float m_headingDegrees;
    float m_pitchDegrees;
    float m_fovx;
    float m_znear;
    float m_zfar;
    Vector3 m_eye;
    Vector3 m_target;
    Vector3 m_targetYAxis;
    Vector3 m_xAxis;
    Vector3 m_yAxis;
    Vector3 m_zAxis;
    Vector3 m_viewDir;
    Vector3 m_velocity;
    Matrix4 m_viewMatrix;
    Matrix4 m_projMatrix;
    Quaternion m_orientation;
};

//-----------------------------------------------------------------------------

inline float ThirdPersonCamera::getDampingConstant() const
{ return m_dampingConstant; }

inline float ThirdPersonCamera::getOffsetDistance() const
{ return m_offsetDistance; }

inline const Quaternion &ThirdPersonCamera::getOrientation() const
{ return m_orientation; }

inline const Vector3 &ThirdPersonCamera::getPosition() const
{ return m_eye; }

inline const Matrix4 &ThirdPersonCamera::getProjectionMatrix() const
{ return m_projMatrix; }

inline float ThirdPersonCamera::getSpringConstant() const
{ return m_springConstant; }

inline const Vector3 &ThirdPersonCamera::getTargetYAxis() const
{ return m_targetYAxis; }

inline const Vector3 &ThirdPersonCamera::getViewDirection() const
{ return m_viewDir; }

inline const Matrix4 &ThirdPersonCamera::getViewMatrix() const
{ return m_viewMatrix; }

inline const Vector3 &ThirdPersonCamera::getXAxis() const
{ return m_xAxis; }

inline const Vector3 &ThirdPersonCamera::getYAxis() const
{ return m_yAxis; }

inline const Vector3 &ThirdPersonCamera::getZAxis() const
{ return m_zAxis; }

inline bool ThirdPersonCamera::springSystemIsEnabled() const
{ return m_enableSpringSystem; }

#endif