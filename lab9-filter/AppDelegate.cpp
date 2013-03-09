#include "AppDelegate.h"
#include <GL/glew.h>
#include <stdexcept>

const char MODEL_TANK_PHOENIX_PATH[] = "lab7scene/phoenix_ugv.md2";

class Lab9FilterAppData
{
public:
    GL::FrameBuffer frameBuffer;
    GL::RenderBuffer depthBuffer;
    GL::Texture2D fbTexture;
    std::shared_ptr<GL::StaticModel> tank;
    GL::RenderContext tankCtx;
    GL::RenderContext tankCtx2;
};

Static3DSceneAppDelegateOptions options_lab9_filter()
{
    Static3DSceneAppDelegateOptions opts;
    opts.cameraType = "fly";
    return opts;
}

Lab9FilterAppDelegate::Lab9FilterAppDelegate()
    : Static3DSceneAppDelegate(options_lab9_filter())
    , d(new Lab9FilterAppData)
{
}

Lab9FilterAppDelegate::~Lab9FilterAppDelegate()
{
    delete d;
}

void Lab9FilterAppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    Static3DSceneAppDelegate::willRedisplay(window, deltaTime);

    drawObjectToTexture();
    GL::FrameBuffer::unbind();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupViewportAndProjection(window);
    camera()->install();

    OpenGLHelper::enable2DTextures(true);
    d->fbTexture.bind();

    glBegin(GL_QUADS);
    {
        double W = 4.0;
        double H = 3.0;
        glTexCoord2f(0, 0);
        glVertex3f(0, -W, -H);
        glTexCoord2f(1, 0);
        glVertex3f(0, +W, -H);
        glTexCoord2f(1, 1);
        glVertex3f(0, +W, +H);
        glTexCoord2f(0, 1);
        glVertex3f(0, -W, +H);

        glTexCoord2f(0, 1);
        glVertex3f(0, -W, +H);
        glTexCoord2f(1, 1);
        glVertex3f(0, +W, +H);
        glTexCoord2f(1, 0);
        glVertex3f(0, +W, -H);
        glTexCoord2f(0, 0);
        glVertex3f(0, -W, -H);
    }
    glEnd();

    d->tank->render(d->tankCtx2);
}

void Lab9FilterAppDelegate::didInitialized(const Window &window)
{
    Static3DSceneAppDelegate::didInitialized(window);

    OpenGLHelper::enableLights(false);
    camera()->loadLookAtRH(vec3d(10, 2, 0),
                           vec3d(0, 0, 0),
                           vec3d(0, 0, 1));

    d->frameBuffer.setWidth(800);
    d->frameBuffer.setHeight(600);
    d->frameBuffer.initAndBind();
    d->fbTexture = GL::Texture2D::createLinkedWith(d->frameBuffer);
    d->depthBuffer = GL::RenderBuffer::createLinkedWith(d->frameBuffer);
    d->frameBuffer.checkStatus();
    GL::FrameBuffer::unbind();

    d->tankCtx2.m_transform.scaleAllSides(0.2);
    d->tankCtx2.m_transform.translate(vec3d(30, 0, 0));

    try {
        loadModel1();
    } catch (const std::runtime_error &ex) {
        puts(ex.what());
        fflush(stdout);
        exit(1);
    }
    OpenGLHelper::dumpIfError();
}

void Lab9FilterAppDelegate::drawObjectToTexture()
{
//    OpenGLHelper::enable2DTextures(false);
    d->frameBuffer.bind();

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    d->frameBuffer.setupViewportAndProjection();

    mat4d modelView;
    modelView.loadLookAtRH(vec3d(15, 1, 2),
                           vec3d(0, 0, 2),
                           vec3d(0, 0, 1));
    OpenGLHelper::setModelViewMatrix(modelView);

    d->tank->render(d->tankCtx);
    OpenGLHelper::renderTestCube();

    d->fbTexture.bind();
    glGenerateMipmapEXT(GL_TEXTURE_2D);
    d->frameBuffer.unbind();
}

void Lab9FilterAppDelegate::setupViewportAndProjection(const Window &window)
{
    glViewport(0, 0, window.width(), window.height());
    double aspect = double(window.width()) / double(window.height());

    mat4d proj;
    proj.loadPerspective(60.0, aspect, 0.05, 100.0);
    OpenGLHelper::setProjectionMatrix(proj);
}

void Lab9FilterAppDelegate::loadModel1()
{
    mat4 fix;
    fix.rotate(180, vec3(0, 0, 1));
    fix.rotate(90, vec3(1, 0, 0));
    fix.scaleAllSides(0.15);
    d->tank.reset(GL::StaticModel::loadByPath(MODEL_TANK_PHOENIX_PATH, fix));
}
