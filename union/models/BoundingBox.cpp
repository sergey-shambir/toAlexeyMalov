#include "BoundingBox.h"

namespace GL {

BoundingBox::BoundingBox()
    : m_isNull(true)
{
}

BoundingBox::BoundingBox(const vec3 &min, const vec3 &max)
    : m_mins(min)
    , m_maxies(max)
    , m_isNull(false)
{
}

BoundingBox::BoundingBox(float minX, float minY, float minZ,
                         float maxX, float maxY, float maxZ)
    : m_mins(minX, minY, minZ)
    , m_maxies(maxX, maxY, maxZ)
    , m_isNull(false)
{
}

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::isNull() const
{
    return m_isNull;
}

void BoundingBox::uniteWith(const BoundingBox &other)
{
    if (m_isNull) {
        m_mins = other.m_mins;
        m_maxies = other.m_maxies;
        m_isNull = other.m_isNull;
    } else {
        m_mins = min(m_mins, other.m_mins);
        m_maxies = max(m_maxies, other.m_maxies);
    }
}

void BoundingBox::uniteWith(float minimums[3], float maximums[3])
{
    if (m_isNull) {
        m_mins = vec3(minimums);
        m_maxies = vec3(maximums);
        m_isNull = false;
    } else {
        m_mins = min(m_mins, vec3(minimums));
        m_maxies = max(m_maxies, vec3(maximums));
    }
}

} // namespace GL
