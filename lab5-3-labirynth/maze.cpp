#include "maze.h"

static const char BLACK = 'b';
static const char WHITE = 'w';

Maze::Maze(int sizeX, int sizeY, float height)
    : m_sizeX(sizeX)
    , m_sizeY(sizeY)
    , m_wallsHeight(height)
    , m_startPosition(sizeX * 0.5, sizeY * 0.5, height * 0.5)
{
    m_colors.resize(sizeX * sizeY, WHITE);
    m_walls.reserve(4 * std::max(m_sizeX, m_sizeY));
}

Maze::~Maze()
{
}

int Maze::sizeX() const
{
    return m_sizeX;
}

int Maze::sizeY() const
{
    return m_sizeY;
}

double Maze::wallsHeight() const
{
    return m_wallsHeight;
}

vec3d Maze::startPosition() const
{
    return m_startPosition;
}

bool Maze::isBlack(int x, int y) const
{
    return isBlackImpl(x, y);
}

const std::vector<MazeWall> &Maze::walls() const
{
    return m_walls;
}

void Maze::setBlack(int x, int y)
{
    setBlackImpl(x, y);
}

void Maze::setStartPosition(int x, int y)
{
    m_startPosition.x = x;
    m_startPosition.y = y;
}

namespace {
enum Transition {
    Transition_NONE,
    Transition_WHITE, // to white
    Transition_BLACK // to black
};
}

void Maze::createWalls()
{
    MazeWall wall;

    Transition curr = Transition_NONE;
    // horizontal
    for (int x = 1; x < m_sizeX; ++x) {
        Transition prev = Transition_NONE;
        for (int y = 0; y < m_sizeY; ++y) {
            if (isBlack(x - 1, y)) {
                curr = isBlack(x, y) ? Transition_NONE : Transition_WHITE;
            } else {
                curr = isBlack(x, y) ? Transition_BLACK : Transition_NONE;
            }
            if (curr != prev) {
                if (prev != Transition_NONE) {
                    // save end points and fflush
                    wall.verts[2].z = 0;
                    wall.verts[3].z = m_wallsHeight;
                    wall.verts[2].x = wall.verts[3].x = x - 0.5f;
                    wall.verts[2].y = wall.verts[3].y = y - 0.5f;
                    // tex coords
                    const float diffY(wall.verts[2].y - wall.verts[1].y);
                    wall.texCoords[2] = vec2(diffY, 1.0);
                    wall.texCoords[3] = vec2(diffY, 0.0);
                    wall.variant = rand() % WALLS_VARIANTS;
                    m_walls.push_back(wall);
                }
                if (curr != Transition_NONE) {
                    // initialize new wall
                    wall.verts[1].z = 0;
                    wall.verts[0].z = m_wallsHeight;
                    wall.verts[0].x = wall.verts[1].x = x - 0.5f;
                    wall.verts[0].y = wall.verts[1].y = y - 0.5f;
                    // tex coords
                    wall.texCoords[0] = vec2(0.0, 0.0);
                    wall.texCoords[1] = vec2(0.0, 1.0);
                    wall.normal = vec3((curr == Transition_BLACK) ? 1 : -1, 0, 0);
                }
                prev = curr;
            }
        }
    }

    curr = Transition_NONE;
    // vertical
    for (int y = 1; y < m_sizeY; ++y) {
        Transition prev = Transition_NONE;
        for (int x = 0; x < m_sizeX; ++x) {
            if (isBlack(x, y - 1)) {
                curr = isBlack(x, y) ? Transition_NONE : Transition_WHITE;
            } else {
                curr = isBlack(x, y) ? Transition_BLACK : Transition_NONE;
            }
            if (curr != prev) {
                if (prev != Transition_NONE) {
                    // save end points and fflush
                    wall.verts[2].z = 0;
                    wall.verts[3].z = m_wallsHeight;
                    wall.verts[2].x = wall.verts[3].x = x - 0.5f;
                    wall.verts[2].y = wall.verts[3].y = y - 0.5f;
                    // tex coords
                    const float diffX(wall.verts[2].x - wall.verts[1].x);
                    wall.texCoords[2] = vec2(diffX, 1.0);
                    wall.texCoords[3] = vec2(diffX, 0.0);
                    wall.variant = rand() % WALLS_VARIANTS;
                    m_walls.push_back(wall);
                }
                if (curr != Transition_NONE) {
                    // initialize new wall
                    wall.verts[1].z = 0;
                    wall.verts[0].z = m_wallsHeight;
                    wall.verts[0].x = wall.verts[1].x = x - 0.5f;
                    wall.verts[0].y = wall.verts[1].y = y - 0.5f;
                    // tex coords
                    wall.texCoords[0] = vec2(0.0, 0.0);
                    wall.texCoords[1] = vec2(0.0, 1.0);
                    wall.normal = vec3(0, (curr == Transition_BLACK) ? -1 : 1, 0);
                }
                prev = curr;
            }
        }
    }

    std::sort(m_walls.begin(), m_walls.end());
}

void Maze::setBlackImpl(int x, int y)
{
    m_colors[x + m_sizeX * y] = BLACK;
}

bool Maze::isBlackImpl(int x, int y) const
{
    return (m_colors[x + m_sizeX * y] == BLACK);
}
