#pragma once

namespace GL {

class ModelInterface;
class BoundingBox;

class AnimatorInterface
{
public:
    virtual ~AnimatorInterface();

    virtual void animate(float deltaTime /*seconds*/) = 0;
    virtual const BoundingBox& boundingBox() const = 0;
};

} // namespace GL
