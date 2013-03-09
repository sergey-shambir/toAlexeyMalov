#include "appdelegate.h"
#include "maze.h"
#include "mazeloader.h"
#include <memory>
#include <stdexcept>

#include <GL/glew.h>

static const double Z_NEAR = 0.05;
static const double Z_FAR = 100.0;

typedef Static3DSceneAppDelegate super;

////////////////////////////////////////////////////////////////////////////////

class CameraDelegate : public GL::IFirstPersonCameraDelegate
{
public:
    Maze &m_maze;

    CameraDelegate(Maze &maze) : m_maze(maze) {}

    vec3d prepareFixPosition(const vec3d &position)
    {
        vec3d ret(position);
        int x = round(ret.x);
        int y = round(ret.y);
        if (m_maze.isBlack(x, y)) {
            if (ret.x > x) {
                ret.x = x + 0.5;
            } else {
                ret.x = x - 0.5;
            }
            if (ret.y > y) {
                ret.y = y + 0.5;
            } else {
                ret.y = y - 0.5;
            }
        }

        return ret;
    }

    vec3d fixPosition(const vec3d &position)
    {
        vec3d temp = prepareFixPosition(position);
        vec3d variants[4] = {temp, temp, temp, temp};
        variants[0].x += 0.2;
        variants[1].x -= 0.2;
        variants[2].y += 0.2;
        variants[3].y -= 0.2;

        int counter = 0;
        vec3d sum;
        for (vec3d vec : variants) {
            int x = round(vec.x);
            int y = round(vec.y);
            if (!m_maze.isBlack(x, y)) {
                sum += vec;
                ++counter;
            }
        }

        if (counter == 0)
            return position;
        return sum / static_cast<double>(counter);
    }
};

////////////////////////////////////////////////////////////////////////////////

class DataOfAppDelegate
{
public:
    std::shared_ptr<Maze> m_maze;
    std::unique_ptr<CameraDelegate> m_camDelegate;
    GL::OmniLight m_lgtLamp0;
    GL::PhongMaterial m_matWallSurface;
    GL::Texture2D m_texWalls[WALLS_VARIANTS];
    GL::Program m_prImprovedPipeline;

    DataOfAppDelegate()
        : m_lgtLamp0(vec3(0, 0, 0))
    {
        m_lgtLamp0.setDiffuse(vec4(1, 1, 1, 1));
        m_lgtLamp0.setAmbient(vec4(1, 1, 1, 1));
        m_lgtLamp0.setSpecular(vec4(1, 1, 1, 1));
        m_lgtLamp0.setLinearAttenuation(0.02);
        m_matWallSurface.setAmbient(vec4(0.15, 0.16, 0.17, 1));
        m_matWallSurface.setDiffuse(vec4(0.5, 0.53, 0.55, 1));
        m_matWallSurface.setSpecular(vec4(0.08, 0.02, 0.08, 1));
        m_matWallSurface.setShininess(10.0);
    }
};

////////////////////////////////////////////////////////////////////////////////

static Static3DSceneAppDelegateOptions defaultAppOptions()
{
    Static3DSceneAppDelegateOptions ret;
    ret.cameraType = "walk";
    ret.zFar = 100.0f;
    ret.zNear = 0.1f;
    return ret;
}

////////////////////////////////////////////////////////////////////////////////

AppDelegate::AppDelegate(const std::string &imagePath)
    : Static3DSceneAppDelegate(defaultAppOptions())
    , d(new DataOfAppDelegate)
{
    MazeLoader loader(imagePath.c_str());
    d->m_maze = loader.maze();
    d->m_camDelegate.reset(new CameraDelegate(*d->m_maze));
}

AppDelegate::~AppDelegate()
{
    delete d;
}

void AppDelegate::willRedisplay(const Window &window, float deltaTime)
{
    super::willRedisplay(window, deltaTime);

    d->m_prImprovedPipeline.use();
    d->m_matWallSurface.install();

    Maze &maze = *(d->m_maze);

    glBegin(GL_QUADS);
    {
        unsigned variant = static_cast<unsigned>(-1);
        for (const MazeWall &wall : maze.walls()) {
            if (wall.variant != variant) {
                glEnd();
                variant = wall.variant;
                d->m_texWalls[variant].bind();
                glBegin(GL_QUADS);
            }
            glNormal3fv(wall.normal);
            for (int i(0); i < 4; ++i) {
                glTexCoord2f(wall.texCoords[i].x, wall.texCoords[i].y);
                glVertex3fv(wall.verts[i]);
            }
        }
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        unsigned variant = static_cast<unsigned>(-1);
        for (const MazeWall &wall : maze.walls()) {
            if (wall.variant != variant) {
                glEnd();
                variant = wall.variant;
                d->m_texWalls[variant].bind();
                glBegin(GL_QUADS);
            }
            vec3 normal = -wall.normal;
            glNormal3fv(normal);
            for (int i(3); i >= 0; --i) {
                glTexCoord2f(wall.texCoords[i].x, wall.texCoords[i].y);
                glVertex3fv(wall.verts[i]);
            }
        }
    }
    glEnd();

    OpenGLHelper::dumpIfError();
}

void AppDelegate::didInitialized(const Window &window)
{
    super::didInitialized(window);

    OpenGLHelper::enable2DTextures(true);
    OpenGLHelper::dumpIfError();

    vec3d pos = d->m_maze->startPosition();
    camera()->loadLookAtRH(pos, pos + vec3d(1, 0, 0), vec3d(0, 0, 1));
    auto fpCamera = dynamic_cast<GL::FirstPersonCamera*>(camera());
    fpCamera->setDelegate(d->m_camDelegate.get());

    try {
        d->m_texWalls[0] = GL::Texture2D::createFromImageByPath("maze/wall1.png");
        d->m_texWalls[1] = GL::Texture2D::createFromImageByPath("maze/wall2.png");
        d->m_texWalls[2] = GL::Texture2D::createFromImageByPath("maze/wall3.jpeg");
        d->m_texWalls[3] = GL::Texture2D::createFromImageByPath("maze/wall4.jpeg");

        auto vert(GL::Shader::createFromFile("GLSL/pipeline/pipeline.vert", GL_VERTEX_SHADER));
        auto frag(GL::Shader::createFromFile("GLSL/pipeline/pipeline.frag", GL_FRAGMENT_SHADER));
        d->m_prImprovedPipeline = GL::Program::fromShaders2(vert, frag);
        d->m_prImprovedPipeline.autorelease();
    } catch (const std::runtime_error &ex) {
        puts(ex.what());
        fflush(stdout);
        exit(1);
    }

    glEnable(GL_LIGHT0);
    glPushMatrix();
    OpenGLHelper::setModelViewMatrix(mat4d());
    d->m_lgtLamp0.render(GL_LIGHT0);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////


void AppDelegate::willQuit(const Window &window)
{
    super::willQuit(window);
}
