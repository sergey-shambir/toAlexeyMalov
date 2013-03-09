#pragma once
#include "rectangle.h"
#include <math.h>

template<class treal>
inline treal distance(treal x1, treal y1, treal x2, treal y2)
{
    treal dx = x2 - x1;
    treal dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

template<class treal>
inline bool isInCircle(base::vec2<treal> const& point,
                       base::vec2<treal> const& center,
                       treal radius)
{
    treal dx = center.x - point.x;
    treal dy = center.y - point.y;
    return (dx * dx + dy * dy) <= radius * radius;
}
