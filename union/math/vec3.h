/**

  3D vector incapsulates common operations with points, directions, etc.

  */

#pragma once
#include "vec2.h"
#include <math.h>
#include <algorithm>

namespace base {

template <class treal>
class vec3
{
public:
    treal x;
    treal y;
    treal z;

    inline explicit vec3(treal vx = 0.0, treal vy = 0.0, treal vz = 0.0)
        : x(vx), y(vy), z(vz) { }
    inline explicit vec3(const vec2<treal> &vec, treal vz = 0.0)
        : x(vec.x), y(vec.y), z(vz) { }
    inline explicit vec3(const treal *array)
        : x(array[0]), y(array[1]), z(array[2]) { }

    inline operator treal const*() const
    {
        return &x;
    }

    template <class TRealAnother>
    inline vec3<TRealAnother> convert() const
    {
        return vec3<TRealAnother>(static_cast<TRealAnother>(x),
                                  static_cast<TRealAnother>(y),
                                  static_cast<TRealAnother>(z));
    }

    /** Scale */
    inline vec3 &operator *= (treal scale)
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    inline vec3 operator *(treal value) const
    {
        return vec3(x * value, y * value, z * value);
    }

    inline vec3 operator *(const vec3 &value) const
    {
        return vec3(x * value.x, y * value.y, z * value.z);
    }

    /** Division on scalar */
    inline vec3 &operator /= (treal scalar)
    {
        return *this *= (1/scalar);
    }

    inline vec3 operator / (treal scalar) const
    {
        return (*this) * (1/scalar);
    }

    /** Substract */
    inline vec3 operator -(vec3 const& right) const
    {
        return vec3(x - right.x, y - right.y, z - right.z);
    }

    inline vec3 operator -(treal value) const
    {
        return vec3(x - value, y - value);
    }

    /** Unary minus */
    inline vec3 operator -() const
    {
        return vec3(-x, -y, -z);
    }

    inline vec3 &operator +=(treal addenum)
    {
        x += addenum;
        y += addenum;
        z += addenum;
        return *this;
    }

    inline vec3 &operator +=(const vec3 &addenum)
    {
        x += addenum.x;
        y += addenum.y;
        z += addenum.z;
        return *this;
    }

    inline vec3 operator +(treal addenum) const
    {
        return vec3(x + addenum, y + addenum, z + addenum);
    }

    inline vec3 operator +(const vec3 &addenum) const
    {
        return vec3(x + addenum.x, y + addenum.y, z + addenum.z);
    }

    inline treal length() const
    {
        return static_cast<treal>(sqrt(x * x + y * y + z * z));
    }

    inline treal lengthSq() const
    {
        return static_cast<treal>(x * x + y * y + z * z);
    }

    inline vec3 withoutZ() const
    {
        return vec3(x, y, 0.0);
    }

    inline vec3 normalized() const
    {
        treal invLength = 1.0 / length();
        return vec3(x * invLength, y * invLength, z * invLength);
    }

    inline vec3 &normalize()
    {
        treal invLength = 1.0 / length();
        x *= invLength;
        y *= invLength;
        z *= invLength;
        return *this;
    }

    treal maxLength() const
    {
        return max3(fabs(x), fabs(y), fabs(z));
    }

    vec3 &clamp(treal lower = 0.0, treal upper = 1.0)
    {
        x = std::min(std::max(x, lower), upper);
        y = std::min(std::max(y, lower), upper);
        z = std::min(std::max(z, lower), upper);
        return *this;
    }

    vec3 clamped(treal lower = 0.0, treal upper = 1.0) const
    {
        vec3 ret;
        ret.x = std::min(std::max(x, lower), upper);
        ret.y = std::min(std::max(y, lower), upper);
        ret.z = std::min(std::max(z, lower), upper);
        return ret;
    }

    void clear()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
};

} // namespace base

template<class treal>
inline base::vec3<treal> min(base::vec3<treal> const& a,
                                 base::vec3<treal> const& b)
{
    return base::vec3<treal>(std::min(a.x, b.x),
                             std::min(a.y, b.y),
                             std::min(a.z, b.z));
}

template<class treal>
inline base::vec3<treal> max(base::vec3<treal> const& a,
                                 base::vec3<treal> const& b)
{
    return base::vec3<treal>(std::max(a.x, b.x),
                             std::max(a.y, b.y),
                             std::max(a.z, b.z));
}

template<class treal>
inline base::vec3<treal> const operator*(treal const& scalar, base::vec3<treal> const& v)
{
    return v * scalar;
}

template <class treal>
inline treal dot(const base::vec3<treal> &left, const base::vec3<treal> &right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <class treal>
inline base::vec3<treal> cross(base::vec3<treal> const& a, base::vec3<treal> const& b)
{
    /*
       A  B   :
          |i  j  k |
    AxB = |Ax Ay Az|
          |Bx By Bz|
    */
    return base::vec3<treal>(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x );

}

typedef base::vec3<float> vec3;
typedef base::vec3<double> vec3d;
