#pragma once
#include "vec3.h"
#include <math.h>

/**
  Quaternion class
  */
class quat
{
public:
    float a;
    vec3 u;

    inline explicit quat(float a, const vec3 &u)
        : a(a)
        , u(u)
    {
    }

    inline quat operator +(const quat &other)
    {
        return quat(a + other.a, u + other.u);
    }

    inline quat &operator +=(const quat &other)
    {
        a += other.a;
        u += other.u;
        return *this;
    }

    inline quat operator *(const quat &other)
    {
        return quat(a * other.a - dot(u, other.u),
                    a * other.u + other.a * u + cross(u, other.u));
    }

    inline quat conjugate() const
    {
        return quat(a, -u);
    }

    inline float length() const
    {
        return sqrt(a * a + u.x * u.x + u.y * u.y + u.z * u.z);
    }
};
