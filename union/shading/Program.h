#pragma once
#include <string>
#include <map>
#include "Shader.h"

namespace GL {

class Program
{
public:
    Program();
    virtual ~Program();

    /**
     * @brief Creates and links program with single shader
     * @throws std::runtime_error if cannot link
     */
    static Program fromShaders1(const Shader& first);

    /**
     * @brief Creates and links program with two shaders
     * @throws std::runtime_error if cannot link
     */
    static Program fromShaders2(const Shader& first, const Shader& second);

    /**
     * @brief Validates if program can be used in current OpenGL state
     * @return true if OpenGL state is ready for program
     */
    bool canUse() const;

    /**
     * @brief Passes program to OpenGL state
     */
    virtual void use() const;

    void autorelease();

    int getIntParam(int pname) const;
    int getBoolParam(int pname) const; //! compares value with GL_TRUE

    /**
     * @brief Retrieves uniform location descriptor
     * @param name - uniform name, should not start with "gl_"
     * @return -1 if name not exists or not available, OpenGL location otherwise
     */
    int getUniformLocation(const std::string &name) const;

    /**
     * @brief Can be called after canUse()
     */
    std::string getInfoLog() const;

private:
    /**
     * @brief Inits program using already filled array m_shaders
     * @throws std::runtime_error if cannot link
     */
    void link();
    void release();

    bool m_autorelease;
    unsigned m_program;
    std::vector<unsigned> m_shaders;

    mutable std::map<std::string, int> m_uniformsCache;
    mutable std::map<std::string, int> m_attributesCache;
};

} // namespace GL
