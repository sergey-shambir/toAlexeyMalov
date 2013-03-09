#pragma once
#include <math/vec3.h>

namespace GL {

class BoundingBox
{
    vec3 m_mins;
    vec3 m_maxies;
    bool m_isNull;

public:
    BoundingBox();
    BoundingBox(const vec3& min, const vec3& max);
    BoundingBox(float minX, float minY, float minZ,
                float maxX, float maxY, float maxZ);
    ~BoundingBox();

    bool isNull() const;

    void uniteWith(const BoundingBox &other);
    void uniteWith(float minimums[3], float maximums[3]);
};

} // namespace GL
