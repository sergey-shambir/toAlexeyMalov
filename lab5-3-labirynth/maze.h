#pragma once

#include <vector>
#include <union-all.h>

const unsigned WALLS_VARIANTS = 4;

struct MazeWall
{
    vec3 verts[4];
    vec2 texCoords[4];
    vec3 normal;
    unsigned variant;

    bool operator < (const MazeWall& other) const
    {
        return (variant < other.variant);
    }
};

class Maze
{
public:
    /**
     * @brief Maze
     * @param sizeX - Size in x coordinate
     * @param sizeY - Size in y coordinate
     * @param height - Height of wall (z coordinate)
     */
    Maze(int sizeX, int sizeY, float height);
    ~Maze();

    int sizeX() const;
    int sizeY() const;
    double wallsHeight() const;
    vec3d startPosition() const;

    bool isBlack(int x, int y) const;
    const std::vector<MazeWall> &walls() const;

    void setBlack(int x, int y);
    void setStartPosition(int x, int y);

    /**
     * @brief Creates all walls surfaces, takes much time
     */
    void createWalls();

private:
    inline void setBlackImpl(int x, int y);
    inline bool isBlackImpl(int x, int y) const;

    const int m_sizeX;
    const int m_sizeY;
    const double m_wallsHeight;
    vec3d m_startPosition;
    std::string m_colors;
    std::vector<MazeWall> m_walls;
};
