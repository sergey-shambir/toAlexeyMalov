#include "EyeCamera.h"
#include <assert.h>
#include <GL/glew.h>
#include <AL/al.h>
#include <SDL2/SDL.h>

#include "../helpers/OpenGLHelper.h"
#include "../helpers/OpenALHelper.h"

static const double FIELD_OF_VIEW = 60.0;

namespace GL {

EyeCamera::EyeCamera(int viewportWidth, int viewportHeight)
    : m_viewport(viewportWidth, viewportHeight)
    , m_eye(-1, -1, 0)
    , m_front(0.77, 0.77, 0)
    , m_up(0, 0, 1)
    , m_speed(0, 0, 0)
    , m_trackingEnabled(false)
    , m_firstMovePassed(false)
{
    m_front.normalize();
}

EyeCamera::~EyeCamera()
{
}

void EyeCamera::install() const
{
    mat4d modelView;
    modelView.lookAtRh(m_eye, m_eye + m_front, m_up);
    OpenGLHelper::setModelViewMatrix(modelView);

    OpenALHelper::setListenerCoordinates(m_eye, m_front, m_up);
    OpenALHelper::setListenerSpeed(m_speed);
}

void EyeCamera::loadLookAtRH(vec3d const& eye,
                             vec3d const& at,
                             vec3d const& up)
{
    m_eye = eye;
    m_front = at - eye;
    m_up = up;

    m_front.normalize();
}

void EyeCamera::setViewport(int width, int height)
{
    m_viewport = std::pair<int, int>(width, height);
}

void EyeCamera::getViewport(int &width, int &height)
{
    width = m_viewport.first;
    height = m_viewport.second;
}

void EyeCamera::movedMouse(int dx, int dy)
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
        vec3d left = cross(m_up, m_front);

        double xAngle = - FIELD_OF_VIEW * 4.0 * dx / m_viewport.first;
        mat4d xRotate;
        xRotate.loadRotation(xAngle, m_up);
        m_front = vec3d(xRotate * vec4d(m_front, 0.0));
        m_front.normalize();

        double yAngle = FIELD_OF_VIEW * 4.0 * dy / m_viewport.second;
        mat4d yRotate;
        yRotate.loadRotation(yAngle, left);
        m_front = vec3d(yRotate * vec4d(m_front, 0.0));
        m_up = vec3d(yRotate * vec4d(m_up, 0.0));

        m_front.normalize();
        m_up.normalize();
    }
}

void EyeCamera::setTrackingEnabled(bool enabled)
{
    m_trackingEnabled = enabled;
}

bool EyeCamera::isTrackingEnabled() const
{
    return m_trackingEnabled;
}

vec3d EyeCamera::speed() const
{
    return m_speed;
}

void EyeCamera::setSpeed(const vec3d &value)
{
    m_speed = value;
}

void EyeCamera::move(float deltaTime)
{
    vec3d left = cross(m_up, m_front);
    float dFront = m_speed.x * deltaTime;
    float dLeft = m_speed.y * deltaTime;
    float dUp = m_speed.z * deltaTime;
    m_eye += m_front * dFront;
    m_eye += left * dLeft;
    m_eye += m_up * dUp;
}

const vec3d &EyeCamera::positionOfEye() const
{
    return m_eye;
}

} // namespace GL
