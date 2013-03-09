#pragma once

#include <memory>
class Maze;

class MazeLoader
{
public:
    /**
     * @brief MazeLoader
     * @param imagePath - Path to graiscale image that keeps maze
     * @throws std::runtime_error if image cannot be opened
     *
     * Image should:
     *   1. be small, 300x400 is acceptable
     *   2. have only vertical and horizontal walls
     *   3. have bold walls that cannot be splitted on too big amount of chunks
     *   4. it should be grayscale, and have exactly one red pixel
     */
    MazeLoader(const char *imagePath);
    ~MazeLoader();

    std::shared_ptr<Maze> maze() const;

private:
    std::shared_ptr<Maze> m_maze;
};
