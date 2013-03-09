#include "firstpersoncamera.h"
#include "../math/mat4.h"
#include <assert.h>
#include <GL/glew.h>
#include <AL/al.h>
#include <SDL/SDL.h>

static const double FIELD_OF_VIEW = 60.0;

static bool isModelViewMatrixActive()
{
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE, &mode);
    return (mode == GL_MODELVIEW);
}

namespace GL {

FirstPersonCamera::FirstPersonCamera(int viewportWidth, int viewportHeight)
    : m_viewport(viewportWidth, viewportHeight)
    , m_trackingEnabled(false)
    , m_firstMovePassed(false)
    , m_delegate(nullptr)
{
}

void FirstPersonCamera::install() const
{
    // OpenGL part
    assert(isModelViewMatrixActive());

    mat4d modelView;
    modelView.lookAtRh(m_eye, m_eye + m_front, vec3d(0, 0, 1));
    glLoadMatrixd(modelView);

    // OpenAL part
    ALfloat listenerOri[6] = {(ALfloat)m_front.x, (ALfloat)m_front.y, (ALfloat)m_front.z,
                              (ALfloat)0, (ALfloat)0, (ALfloat)1};
    alListenerfv(AL_POSITION, m_eye.convert<ALfloat>());
    alListenerfv(AL_VELOCITY, base::vec3<ALfloat>(m_speed.first, m_speed.second, 0.0));
    alListenerfv(AL_ORIENTATION, listenerOri);
}

void FirstPersonCamera::loadLookAtRH(const vec3d &eye, const vec3d &at, const vec3d &up)
{
    (void)up;
    m_eye = eye;
    m_front = (at - eye).withoutZ().normalized();
}

void FirstPersonCamera::setViewport(int width, int height)
{
    m_viewport = std::pair<int, int>(width, height);
}

void FirstPersonCamera::getViewport(int &width, int &height)
{
    width = m_viewport.first;
    height = m_viewport.second;
}

void FirstPersonCamera::movedMouse(int dx, int dy)
{
    if (!m_firstMovePassed) {
        /*
         * First move event will be sent on window initialization
         */
        m_firstMovePassed = true;
        dx -= 0.5 * m_viewport.first;
        dy -= 0.5 * m_viewport.second;
    }
    if (m_trackingEnabled) {
//        vec3d left(-m_front.y, m_front.x, 0.0);

        double xAngle = - FIELD_OF_VIEW * 4.0 * dx / m_viewport.first;
        mat4d xRotate;
        xRotate.loadRotation(xAngle, vec3d(0, 0, 1));
        m_front = vec3d(xRotate * vec4d(m_front, 0.0));
        m_front.normalize();

//        double yAngle = FIELD_OF_VIEW * 4.0 * dy / m_viewport.second;
//        mat4d yRotate;
//        yRotate.loadRotation(yAngle, left);
//        m_front = vec3d(yRotate * vec4d(m_front, 0.0));

        m_front.normalize();
    }
}

void FirstPersonCamera::setTrackingEnabled(bool enabled)
{
    m_trackingEnabled = enabled;
}

bool FirstPersonCamera::isTrackingEnabled() const
{
    return m_trackingEnabled;
}

vec3d FirstPersonCamera::speed() const
{
    return vec3d(m_speed.first, m_speed.second, 0.0);
}

void FirstPersonCamera::setSpeed(const vec3d &value)
{
    m_speed.first = value.x;
    m_speed.second = value.y;
}

void FirstPersonCamera::move(float deltaTime)
{
    vec3d left(-m_front.y, m_front.x, 0);
    float dFront = m_speed.first * deltaTime;
    float dLeft = m_speed.second * deltaTime;

    m_eye += m_front * dFront;
    m_eye += left * dLeft;

    if (m_delegate) {
        m_eye = m_delegate->fixPosition(m_eye);
    }
}

const vec3d &FirstPersonCamera::positionOfEye() const
{
    return m_eye;
}

void FirstPersonCamera::setDelegate(IFirstPersonCameraDelegate *delegate)
{
    m_delegate = delegate;
}

} // namespace GL
