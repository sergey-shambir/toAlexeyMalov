#include "StaticMesh.h"
#include <GL/glew.h>

namespace GL {

StaticMesh::StaticMesh()
    : m_materialIndex(0)
    , m_glPrimitiveType(GL_TRIANGLES)
{
}

} // namespace GL
