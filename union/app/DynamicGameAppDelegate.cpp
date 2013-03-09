#include "DynamicGameAppDelegate.h"
#include "Window.h"
#include <GL/glew.h>
#include "../helpers/OpenGLHelper.h"

class DynamicGameAppDelegateData
{
public:
    DynamicGameOpts opts;

    std::vector<vec4> matrix;
    vec4 colorDefault;
    vec4 colorWinner;
    vec4 colorLoser;

    DynamicGameAppDelegateData(const DynamicGameOpts &opts)
        : opts(opts)
        , colorDefault(0.0, 0.0, 0.0, 1)
        , colorWinner(0.3, 1.0, 0.3, 1)
        , colorLoser(1.0, 0.4, 0.4, 1)
    {
        matrix.resize(opts.height * opts.width, colorDefault);
    }

    unsigned index(unsigned x, unsigned y)
    {
        return x + opts.width * y;
    }
};

// FIXME: clang doesn't show diagnsotic here
DynamicGameAppDelegate::DynamicGameAppDelegate(const DynamicGameOpts &opts)
    : d(new DynamicGameAppDelegateData(opts))
{
}

DynamicGameAppDelegate::~DynamicGameAppDelegate()
{
    delete d;
}

void DynamicGameAppDelegate::setCellState(unsigned x, unsigned y, bool winner)
{
    d->matrix[d->index(x, y)] = (winner) ? d->colorWinner : d->colorLoser;
}

void DynamicGameAppDelegate::setWinColor(const vec3 &color)
{
    d->colorWinner = vec4(color, 1.0);
}

void DynamicGameAppDelegate::setLoseColor(const vec3 &color)
{
    d->colorLoser = vec4(color, 1.0);
}

void DynamicGameAppDelegate::setDefaultColor(const vec3 &color)
{
    d->colorDefault = vec4(color, 1.0);
}

void DynamicGameAppDelegate::willRedisplay(const Window &, float deltaTime)
{
    (void)deltaTime;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawField();
}

void DynamicGameAppDelegate::didResize(const Window &)
{
}

void DynamicGameAppDelegate::didInitialized(const Window &window)
{
    glViewport(0, 0, window.width(), window.height());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, d->opts.width, 0, d->opts.height, -1, +1);
    glMatrixMode(GL_MODELVIEW);
    glLineWidth(2);
    OpenGLHelper::dumpIfError();
}

void DynamicGameAppDelegate::drawField()
{
    glBegin(GL_QUADS);
    for (unsigned x = 0; x < d->opts.width; ++x)
        for (unsigned y = 0; y < d->opts.height; ++y) {
            vec4 color = d->matrix[d->index(x, y)];
            glColor4fv(color);
            glVertex2i(x, y);
            glVertex2i(x, y + 1);
            glVertex2i(x + 1, y + 1);
            glVertex2i(x + 1, y);
        }
    glEnd();

    glBegin(GL_LINES);
    for (unsigned x = 0; x < d->opts.width; ++x)
        for (unsigned y = 0; y < d->opts.height; ++y) {
            vec4 color(0, 0, 1, 1);
            glColor4fv(color);
            glVertex2i(x, y);
            glVertex2i(x, y + 1);
            glVertex2i(x, y);
            glVertex2i(x + 1, y);
        }
    glEnd();
}
