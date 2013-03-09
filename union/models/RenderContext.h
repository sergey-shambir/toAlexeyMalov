#pragma once
#include "../math/mat4.h"

namespace GL {

/**
 * @brief GL::RenderContext keeps all data related to object rendering process
 * TODO: add listed things
 *  1) add camera frustum and use existing bounding boxes for optimization
 *  2) add animation part of render context
 */
class RenderContext
{
public:
    mat4d m_transform;
};

} // namespace GL
