#pragma once
#include "vec3.h"

namespace base {

template <class treal>
class vec4
{
public:
    treal x;
    treal y;
    treal z;
    treal w;

    inline explicit vec4(treal vx = 0.0, treal vy = 0.0, treal vz = 0.0, treal vw = 0.0)
        : x(vx), y(vy), z(vz), w(vw)
    {
    }

    inline explicit vec4(vec3<treal> const& vec, treal vw = 0.0)
        : x(vec.x), y(vec.y), z(vec.z), w(vw)
    {
    }

    inline operator treal const*() const
    {
        return &x;
    }

    inline vec4<treal> operator *(treal num) const
    {
        return vec4<treal>(x * num, y * num, z * num, w * num);
    }

    inline treal length() const
    {
        return static_cast<treal>(sqrt(lengthSq()));
    }

    inline treal lengthSq() const
    {
        return static_cast<treal>(x * x + y * y + z * z + w * w);
    }

    inline vec4 normalized() const
    {
        treal invLength = 1.0 / length();
        return vec4(x * invLength, y * invLength, z * invLength, w * invLength);
    }

    inline vec4 &normalize()
    {
        treal invLength = 1.0 / length();
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;
        return *this;
    }
};

template <class treal>
inline vec4<treal> operator *(float num, vec4<treal>vec)
{
    return vec * num;
}

template <class treal>
inline vec4<treal> operator *(double num, vec4<treal>vec)
{
    return vec * num;
}

} // namespace base

typedef base::vec4<float> vec4;
typedef base::vec4<double> vec4d;
