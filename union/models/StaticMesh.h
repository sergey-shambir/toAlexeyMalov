#pragma once
#include "VertexBuffer.h"

namespace GL {

class StaticMesh
{
public:
    VertexBufferRange m_rangeVert;
    VertexBufferRange m_rangeInd;
    unsigned m_materialIndex; /** default: 0 */
    int m_glPrimitiveType; /** default: GL_TRIANGLES */

    StaticMesh();
};

} // namespace GL
