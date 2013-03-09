#include "OverviewAndRotateCamera.h"
#include "../helpers/OpenALHelper.h"
#include "../helpers/OpenGLHelper.h"

static const double FIELD_OF_VIEW = 60.0;

namespace GL {

OverviewAndRotateCamera::OverviewAndRotateCamera(int viewportWidth, int viewportHeight)
    : m_viewport(viewportWidth, viewportHeight)
    , m_eye(-1, -1, 0)
    , m_trackingEnabled(false)
    , m_firstMovePassed(true)
{
    loadLookAtRH(vec3d(10, 10, 0),
                 vec3d(0, 0, 0),
                 vec3d(0, 0, 1));
}

OverviewAndRotateCamera::~OverviewAndRotateCamera()
{
}

void OverviewAndRotateCamera::install() const
{
    mat4d modelView;
    modelView.loadLookAtRH(m_eye,
                           m_eye + m_front,
                           m_up);
    OpenGLHelper::setModelViewMatrix(modelView);

    // TODO: fix OpenAL orientation
    OpenALHelper::setListenerCoordinates(positionOfEye(), vec3d(0.7, 0.7, 0), vec3d(0, 0, 1));
    OpenALHelper::setListenerSpeed(vec3d());
}

void OverviewAndRotateCamera::loadLookAtRH(const vec3d &eye, const vec3d &at, const vec3d &up)
{
    m_eye = eye;
    m_front = (at - eye).normalized();
    m_up = up.normalized();
}

void OverviewAndRotateCamera::setViewport(int width, int height)
{
    m_viewport = std::pair<int, int>(width, height);
}

void OverviewAndRotateCamera::getViewport(int &width, int &height)
{
    width = m_viewport.first;
    height = m_viewport.second;
}

void OverviewAndRotateCamera::movedMouse(int dx, int dy)
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
        double len = m_eye.withoutZ().length();
        double angle = acos(m_eye.x / len);
        double dxAngle = (double(dx) / double(m_viewport.first));
        angle += dxAngle;

        m_eye.x = cos(angle) * len;
        m_eye.y = sin(angle) * len;
    }
}

void OverviewAndRotateCamera::setTrackingEnabled(bool enabled)
{
    m_trackingEnabled = enabled;
}

bool OverviewAndRotateCamera::isTrackingEnabled() const
{
    return m_trackingEnabled;
}

vec3d OverviewAndRotateCamera::speed() const
{
    return vec3d();
}

void OverviewAndRotateCamera::setSpeed(const vec3d &value)
{
    (void)value;
}

void OverviewAndRotateCamera::move(float deltaTime)
{
    (void)deltaTime;
}

const vec3d &OverviewAndRotateCamera::positionOfEye() const
{
    return m_eye;
}

} // namespace GL
