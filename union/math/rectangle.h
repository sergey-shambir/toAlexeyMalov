#pragma once
#include "vec4.h"

namespace base {

template<class treal>
class rectangle
{
public:
    //! attributes
    treal x;
    treal y;
    treal width;
    treal height;

    //! constructors
    explicit rectangle(treal vx = 0.0, treal vy = 0.0, treal vwidth = 0.0, treal vheight = 0.0)
        : x(vx), y(vy), width(vwidth), height(vheight)
    {
    }

    explicit rectangle(vec4<float> const& values)
        : x(values.x), y(values.y), width(values.z), height(values.w)
    {
    }

    explicit rectangle(vec4<double> const& values)
        : x(values.x), y(values.y), width(values.z), height(values.w)
    {
    }

    explicit rectangle(const float *values)
        : x(values[0]), y(values[1]), width(values[2]), height(values[3])
    {
    }

    explicit rectangle(const double *values)
        : x(values[0]), y(values[1]), width(values[2]), height(values[3])
    {
    }

    //! methods
    inline bool isInside(treal px, treal py) const
    {
        return (px >= x) && (py >= y) && (px <= x + width) && (py <= y + height);
    }

    inline bool isOutside(treal px, treal py) const
    {
        return !isInside(px, py);
    }

    inline bool isOnBound(treal px, treal py) const
    {
        return (x == px) || (y == py) || (x + width == px) || (y + height == py);
    }

    inline treal diagonalLength() const
    {
        return sqrt(width * width + height * height);
    }
};

} // namespace base

typedef base::rectangle<float> rectangle;
typedef base::rectangle<double> rectangled;
