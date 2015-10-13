#include "AppDelegate.h"
#include <stdexcept>

const char MODEL_TANK_PHOENIX_PATH[] = "../../Data/lab7scene/phoenix_ugv.md2";
const char MODEL_HELI_PATH[] = "../../Data/lab7scene/helicopter.md2";
const char MODEL_CAR_PATH[] = "../../Data/lab7scene/car.md2";

////////////////////////////////////////////////////////////////////////////////

class DataOfAppDelegate
{
public:
    GL::OmniLight m_lgtLamp0;
    GL::OmniLight m_lgtLamp1;
    GL::PhongMaterial m_matSurface;
    GL::Program m_prImprovedPipeline;
    std::shared_ptr<GL::StaticModel> m_modelPhoenix;
    std::shared_ptr<GL::StaticModel> m_modelHeli;
    std::shared_ptr<GL::StaticModel> m_modelCar;

    GL::RenderContext m_model0Ctx;
    GL::RenderContext m_model1Ctx;
    GL::RenderContext m_model2Ctx;

    DataOfAppDelegate()
        : m_lgtLamp0(vec3(10, 10, 10))
        , m_lgtLamp1(vec3(-10, 10, 10))
    {
        const vec4 COLOR0(0.4, 0.4, 0.4, 1);
        const vec4 COLOR1(0.4, 0.4, 0.4, 1);
        m_lgtLamp0.setDiffuse(COLOR0);
        m_lgtLamp0.setAmbient(COLOR0);
        m_lgtLamp0.setSpecular(COLOR0);

        m_lgtLamp1.setDiffuse(COLOR1);
        m_lgtLamp1.setAmbient(COLOR1);
        m_lgtLamp1.setSpecular(COLOR1);

        m_matSurface.setAmbient(vec4(0.18, 0.20, 0.14, 1));
        m_matSurface.setDiffuse(vec4(0.7, 0.83, 0.55, 1));
        m_matSurface.setSpecular(vec4(0.08, 0.08, 0.02, 1));
        m_matSurface.setShininess(10.0);

        m_model0Ctx.m_transform.scaleAllSides(0.09);
        m_model0Ctx.m_transform.translate(100, 90, 0);

        m_model1Ctx.m_transform.translate(-5, 9, 0);

        m_model2Ctx.m_transform.scaleAllSides(0.02);
        m_model2Ctx.m_transform.translate(50, 50, 50);
    }
};

////////////////////////////////////////////////////////////////////////////////

static Static3DSceneAppDelegateOptions defaultAppOptions()
{
    Static3DSceneAppDelegateOptions ret;
    ret.cameraType = "fly";
    ret.zFar = 100.0f;
    ret.zNear = 0.1f;
    ret.cameraSpeed *= 2;
    return ret;
}

////////////////////////////////////////////////////////////////////////////////

AppDelegate::AppDelegate()
    : Static3DSceneAppDelegate(defaultAppOptions())
    , d(new DataOfAppDelegate)
{
}

AppDelegate::~AppDelegate()
{
    delete d;
}

void AppDelegate::didInitialized(const Window &window)
{
    Static3DSceneAppDelegate::didInitialized(window);
    OpenGLHelper::enable2DTextures(true);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    camera()->loadLookAtRH(vec3d(20, 0, 0),
                           vec3d(0, 0, 0),
                           vec3d(0, 0, 1));

    try {
        auto vert(GL::Shader::createFromFile("../../Data/GLSL/pipeline/pipeline.vert", GL_VERTEX_SHADER));
        auto frag(GL::Shader::createFromFile("../../Data/GLSL/pipeline/pipeline.frag", GL_FRAGMENT_SHADER));
        d->m_prImprovedPipeline = GL::Program::fromShaders2(vert, frag);
        d->m_prImprovedPipeline.autorelease();
        loadModel0();
        loadModel1();
        loadModel2();
    } catch (const std::runtime_error &ex) {
        puts(ex.what());
        fflush(stdout);
        exit(1);
    }
    OpenGLHelper::dumpIfError();
}

void AppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    Static3DSceneAppDelegate::willRedisplay(window, deltaTime);

    // TODO: установи модель фонаря и освети сцену из него с помощью прожекторного источника

//    d->m_prImprovedPipeline.use();

    d->m_lgtLamp0.render(GL_LIGHT0);
    d->m_lgtLamp1.render(GL_LIGHT1);
    d->m_matSurface.install();

    d->m_modelPhoenix->render(d->m_model0Ctx);
    d->m_modelHeli->render(d->m_model1Ctx);
    d->m_modelCar->render(d->m_model2Ctx);

    OpenGLHelper::dumpIfError();
}

void AppDelegate::loadModel0()
{
    mat4 fix;
    fix.rotate(-90, vec3(0, 0, 1));
    fix.rotate(90, vec3(1, 0, 0));
    d->m_modelPhoenix.reset(GL::StaticModel::loadByPath(MODEL_TANK_PHOENIX_PATH, fix));
}

void AppDelegate::loadModel1()
{
    mat4 fix;
    fix.rotate(180, vec3(0, 0, 1));
    fix.rotate(90, vec3(1, 0, 0));
    fix.scaleAllSides(0.15);
    d->m_modelHeli.reset(GL::StaticModel::loadByPath(MODEL_HELI_PATH, fix));
}

void AppDelegate::loadModel2()
{
    mat4 fix;
    fix.rotate(-90, vec3(0, 0, 1));
    fix.rotate(90, vec3(1, 0, 0));
    d->m_modelCar.reset(GL::StaticModel::loadByPath(MODEL_CAR_PATH, fix));
}
