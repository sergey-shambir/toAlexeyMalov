#pragma once

namespace base {
template <class treal>
class vec2
{
public:
    treal x;
    treal y;

    inline explicit vec2(treal vx = 0.0, treal vy = 0.0)
        : x(vx), y(vy) { }
    inline explicit vec2(treal *array)
        : x(array[0]), y(array[1]) { }

    inline vec2 &operator +=(treal addenum)
    {
        x += addenum;
        y += addenum;
        return *this;
    }

    inline vec2 &operator +=(const vec2 &addenum)
    {
        x += addenum.x;
        y += addenum.y;
        return *this;
    }

    inline vec2 operator +(treal addenum)
    {
        return vec2(x + addenum, y + addenum);
    }

    inline vec2 operator +(const vec2 &addenum)
    {
        return vec2(x + addenum.x, y + addenum.y);
    }

    inline vec2 operator -(treal value)
    {
        return vec2(x - value, y - value);
    }

    inline vec2 operator -(const vec2 &value)
    {
        return vec2(x - value.x, y - value.y);
    }

    inline vec2 operator *(treal value)
    {
        return vec2(x * value, y * value);
    }

    inline vec2 operator *(const vec2 &value)
    {
        return vec2(x * value.x, y * value.y);
    }

    inline vec2 operator /(treal value)
    {
        return vec2(x / value, y / value);
    }

    inline vec2 operator /(const vec2 &value)
    {
        return vec2(x / value.x, y / value.y);
    }

    inline treal length() const
    {
        return sqrt(x * x + y * y);
    }

    inline vec2 normalized() const
    {
        treal len = length();
        return vec2(x / len, y / len);
    }

    inline vec2 &normalize()
    {
        treal len = length();
        x /= len;
        y /= len;
        return *this;
    }

    inline treal distance(vec2 const& value) const
    {
        treal dx = x - value.x;
        treal dy = y - value.y;
        return sqrt(dx * dx + dy * dy);
    }
};

} // namespace base

template <class treal>
float dot(const base::vec2<treal> &left, const base::vec2<treal> &right)
{
    return left.x * right.x + left.y * right.y;
}

typedef base::vec2<float> vec2;
typedef base::vec2<double> vec2d;
