#pragma once

namespace GL {

class RenderContext;
class BoundingBox;

class ModelInterface
{
public:
    virtual ~ModelInterface();

    virtual void render(const RenderContext &context) = 0;
    virtual const BoundingBox& boundingBox() const = 0;
};

} // namespace GL
