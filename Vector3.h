///*
//*  Created by Amr Elsehemy on 3/21/10.
//*  Copyright 2010 Elsehemy Dev. All rights reserved.
//*/
//#include <math.h>
//
//class Vector3
//{
//public:
//    float x;
//    float y;
//    float z;
//
//    Vector3()
//    {
//        x = y = z = 0.0f;
//    }
//    Vector3(float x, float y, float z)
//    {
//        this->x = x;
//        this->y = y;
//        this->z = z;
//    }
//
//    void operator += (const Vector3& v)
//    {
//        x += v.x;
//        y += v.y;
//        z += v.z;
//    }
//    Vector3 operator + (const Vector3& v) const
//    {
//        return Vector3(x+v.x, y+v.y, z+v.z);
//    }
//
//    Vector3 operator + (float value) const
//    {
//        return Vector3(x+value, y+value, z+value);
//    }
//
//    void operator -= (const Vector3& v)
//    {
//        x -= v.x;
//        y -= v.y;
//        z -= v.z;
//    }
//
//    Vector3 operator - (const Vector3& v) const
//    {
//        return Vector3(x-v.x, y-v.y, z-v.z);
//    }
//
//    Vector3 operator - (float value) const
//    {
//        return Vector3(x-value, y-value, z-value);
//    }
//
//    /** Multiplies this vector by the given scalar. */
//    void operator *= (const float value)
//    {
//        x *= value;
//        y *= value;
//        z *= value;
//    }
//
//    /** Returns a copy of this vector scaled the given value. */
//    Vector3 operator * (const float value) const
//    {
//        return Vector3(x*value, y*value, z*value);
//    }
//
//    /** Returns a copy of this vector scaled the given value. */
//    Vector3 operator/(const float value) const
//    {
//        return Vector3(x/value, y/value, z/value);
//    }
//
//
//    /** Checks if the two vectors have identical components. */
//    bool operator==(const Vector3& other) const
//    {
//        return x == other.x &&
//            y == other.y &&
//            z == other.z;
//    }
//
//    /** Checks if the two vectors have non-identical components. */
//    bool operator!=(const Vector3& other) const
//    {
//        return !(*this == other);
//    }
//
//    /**
//    * Checks if this vector is component-by-component less than
//    * the other.
//    *
//    * @note This does not behave like a single-value comparison:
//    * !(a < b) does not imply (b >= a).
//    */
//    bool operator<(const Vector3& other) const
//    {
//        return x < other.x && y < other.y && z < other.z;
//    }
//
//    /**
//    * Checks if this vector is component-by-component less than
//    * the other.
//    *
//    * @note This does not behave like a single-value comparison:
//    * !(a < b) does not imply (b >= a).
//    */
//    bool operator>(const Vector3& other) const
//    {
//        return x > other.x && y > other.y && z > other.z;
//    }
//
//    /**
//    * Checks if this vector is component-by-component less than
//    * the other.
//    *
//    * @note This does not behave like a single-value comparison:
//    * !(a <= b) does not imply (b > a).
//    */
//    bool operator<=(const Vector3& other) const
//    {
//        return x <= other.x && y <= other.y && z <= other.z;
//    }
//
//    /**
//    * Checks if this vector is component-by-component less than
//    * the other.
//    *
//    * @note This does not behave like a single-value comparison:
//    * !(a <= b) does not imply (b > a).
//    */
//    bool operator>=(const Vector3& other) const
//    {
//        return x >= other.x && y >= other.y && z >= other.z;
//    }
//
//    /** Zero all the components of the vector. */
//    void clear()
//    {
//        x = y = z = 0;
//    }
//
//    /** Flips all the components of the vector. */
//    void invert()
//    {
//        x = -x;
//        y = -y;
//        z = -z;
//    }
//
//    void Scale(float s)
//    {
//        x = s * x;
//        y = s * y;
//        z = s * z;
//    }
//
//    static Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)
//    {
//        //Use line segment parametric equation: p = p0 + t(p1 - p0);
//        return v0 + (v1 - v0) * t;
//    }
//
//    static float DotProduct(Vector3 v1, Vector3 v2)
//    {
//        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
//    }
//
//    float getMagnitudeSquare()
//    {
//        return x*x+y*y+z*z;
//    }
//    float Length()
//    {
//        return sqrtf(x*x+y*y+z*z);
//    }
//
//    void Invert()
//    {
//        x = -x;
//        y = -y;
//        z = -z;
//    }
//
//    void RotateAroundYAxis(float theta)
//    {
//        float radians = theta * 3.1415926535f / 180.0f;
//        float cosTheta = cos(radians);
//        float sinTheta = sin(radians);
//
//        Vector3 c0(cosTheta, 0, sinTheta);
//        Vector3 c1(0, 1, 0);
//        Vector3 c2(-sinTheta, 0, cosTheta);
//        Vector3 v = *this;
//
//        x = Vector3::DotProduct(v, c0);
//        y = Vector3::DotProduct(v, c1);
//        z = Vector3::DotProduct(v, c2);
//    }
//
//    void RotateAroundXAxis(float theta)
//    {
//        float radians = theta * 3.1415926535f / 180.0f;
//        float cosTheta = cos(radians);
//        float sinTheta = sin(radians);
//
//        Vector3 c0(1, 0, 0);
//        Vector3 c1(0, cosTheta, -sinTheta);
//        Vector3 c2(0, sinTheta, cosTheta);
//        Vector3 v = *this;
//
//        x = Vector3::DotProduct(v, c0);
//        y = Vector3::DotProduct(v, c1);
//        z = Vector3::DotProduct(v, c2);
//    }
//
//    void RotateAroundZAxis(float theta)
//    {
//        float radians = theta * 3.1415926535f / 180.0f;
//        float cosTheta = cos(radians);
//        float sinTheta = sin(radians);
//
//        Vector3 c0(cosTheta, -sinTheta, 0);
//        Vector3 c1(sinTheta, cosTheta, 0);
//        Vector3 c2(0, 0, 1);
//        Vector3 v = *this;
//
//        x = Vector3::DotProduct(v, c0);
//        y = Vector3::DotProduct(v, c1);
//        z = Vector3::DotProduct(v, c2);
//    }
//
//
//    void Normalize()
//    {
//        float l = Length();
//        if (l > 0)
//        {
//            (*this) *= ((float)1)/l;
//        }
//    }
//
//    void Set(float x, float y, float z)
//    {
//        this->x = x;
//        this->y = y;
//        this->z = z;
//    }
//
//    void Set(Vector3 v)
//    {
//        Set(v.x, v.y, v.z);
//    }
//
//    static Vector3 CrossProduct(const Vector3 &v1,  const Vector3 &v2) 
//    {
//        return Vector3(
//            v1.y*v2.z - v1.z*v2.y,
//            -(v1.x*v2.z - v1.z*v2.x),
//            v1.x*v2.y - v1.y*v2.x);
//    }
//
//};
