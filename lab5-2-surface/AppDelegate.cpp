#include "AppDelegate.h"

static const double NPI = 3.1415926;
static const int RESOLUTION = 100;
static const double DELTA = 0.0001;

typedef Static3DSceneAppDelegate super;

AppDelegate::AppDelegate()
    : m_lamp0(vec3(7, 7, 10))
    , m_lamp1(vec3(-7, -7, -10))
{
}

//! Самая мякотка
static vec3d mebiusFunc(double u, double v)
{
    double temp = (1.0f + 0.5 * v * cos(0.5 * u));
    double x = cos(u) * temp;
    double y = sin(u) * temp;
    double z = 0.5 * v * sin(0.5 * u);

    return vec3d(x, y, z);
}

void AppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    super::willRedisplay(window, deltaTime);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    m_lamp0.render(GL_LIGHT0);
    m_lamp1.render(GL_LIGHT1);
    m_surfaceMaterial.install();

    // fix normals
    for (size_t i = 0; i < m_normales.size(); ++i) {
        vec3d direction = m_verticies[i] - getEyePosition();
        double product = dot(m_normales[i], direction);
        if (product < -DELTA)
            m_normales[i] = -m_normales[i];
    }

    glCullFace(GL_BACK);
    drawSurface();
    glCullFace(GL_FRONT);
    drawSurface();
}

void AppDelegate::didInitialized(const Window &window)
{
    super::didInitialized(window);

    m_lamp0.setDiffuse(vec4(0, 1, 1, 1));
    m_lamp0.setAmbient(vec4(0, 1, 1, 1));
    m_lamp0.setSpecular(vec4(0, 1, 1, 1));
    m_lamp1.setDiffuse(vec4(1, 1, 0, 1));
    m_lamp1.setAmbient(vec4(1, 1, 0, 1));
    m_lamp1.setSpecular(vec4(1, 1, 0, 1));
    m_surfaceMaterial.setAmbient(vec4(0.18, 0.15, 0.12, 1));
    m_surfaceMaterial.setDiffuse(vec4(0.6, 0.5, 0.4, 1));
    m_surfaceMaterial.setSpecular(vec4(1.0, 0.4, 0.4, 1));
    m_surfaceMaterial.setShininess(10.0);

    m_verticies.reserve(RESOLUTION * RESOLUTION);
    m_normales.reserve(RESOLUTION * RESOLUTION);
    double stepU = (2 * NPI) / static_cast<double>(RESOLUTION - 1);
    double stepV = 2.0 / static_cast<double>(RESOLUTION - 1);

    for (double u = 0.0; u < 2 * NPI + 0.5 * stepU; u += stepU) {
        for (double v = -1; v < 1 + 0.5 * stepV; v += stepV) {
            vec3d vertex = mebiusFunc(u, v);
            vec3d dv = mebiusFunc(u + DELTA, v) - vertex;
            vec3d du = mebiusFunc(u, v + DELTA) - vertex;
            vec3d normal = cross(du, dv);
            normal.normalize();

            m_verticies.push_back(vertex);
            m_normales.push_back(normal);
        }
    }

    m_indicies.reserve(2 * RESOLUTION * RESOLUTION);
    for (int x = 0; x < RESOLUTION - 1; ++x) {
        for (int y = 0; y < RESOLUTION; ++y) {
            m_indicies.push_back(x + RESOLUTION * y);
            m_indicies.push_back(1 + x + RESOLUTION * y);
        }
    }
}


void AppDelegate::drawSurface()
{
    glVertexPointer(3, GL_DOUBLE, 0, m_verticies.front());
    glNormalPointer(GL_DOUBLE, 0, m_normales.front());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glDrawElements(GL_TRIANGLE_STRIP, m_indicies.size(), GL_UNSIGNED_INT, m_indicies.data());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    OpenGLHelper::dumpIfError();
}
