#include "mazeloader.h"
#include "maze.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdexcept>

static const float MAZE_WALL_HEIGHT = 5.0f;

/**
 * @brief getPixel
 * @param surface - NULL not expected
 * @param x [in] - x coordinate in range [0..width-1]
 * @param y [in] - y coordinate in range [0..height-1]
 * @return RGB vector, each value in range [0..1];
 */
inline static Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    const int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    const Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        throw std::runtime_error("Image have incorrect format, should have 2, 3 or 4 bytes per pixel");
    }
}

inline static vec3 getPixelColor(SDL_Surface *surface, int x, int y)
{
    Uint32 pixel = getPixel(surface, x, y);
    Uint8 pixels[3];
    SDL_GetRGB(pixel, surface->format, &pixels[0], &pixels[1], &pixels[2]);

    const double factor(1.0f / 255.0f);
    return vec3(pixels[0] * factor, pixels[1] * factor, pixels[2] * factor);
}

MazeLoader::MazeLoader(const char *imagePath)
{
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface)
        throw std::runtime_error(std::string("Cannot open image") + imagePath);

    m_maze.reset(new Maze(surface->w, surface->h, MAZE_WALL_HEIGHT));
    Maze &maze = *m_maze;

    for (int x = 0; x < surface->w; ++x) {
        for (int y = 0; y < surface->h; ++y) {
            vec3 color = getPixelColor(surface, x, y);
            if (color.x <= 0.01 && color.y <= 0.01 && color.z <= 0.01) {
                maze.setBlack(x, y);
            } else if (color.x >= 0.9f && color.y <= 0.1f && color.z <= 0.1f) {
                maze.setStartPosition(x, y);
            }
        }
        fflush(stdout);
    }

    maze.createWalls();
}

MazeLoader::~MazeLoader()
{
}

std::shared_ptr<Maze> MazeLoader::maze() const
{
    return m_maze;
}
