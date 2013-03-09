#pragma once
#include <string>
#include <vector>

namespace GL {

class Program;

class Shader
{
public:
    Shader();
    ~Shader();

    /**
     * @brief Creates compiled shader from content of file
     * @param path - path to GLSL file
     * @param oglShaderType - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.
     * @return unmanaged shader that should be released manually
     */
    static Shader createFromFile(const std::string& path, int oglShaderType);

    /**
     * @brief Creates compiled shader from content of string
     * @param code - string with GLSL code
     * @param oglShaderType - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.
     * @return unmanaged shader that should be released manually
     */
    static Shader createFromAscii(const std::string& code, int oglShaderType);

    /**
     * @brief After call, will free OpenGL resource in destructor
     */
    void autorelease();

    /** OpenGL interaction */
    int getIntParameter(int pname) const;
    bool getBoolParameter(int pname) const; //! compares value with GL_TRUE
    std::string getInfoLog() const;

private:
    friend class Program;

    static Shader fromGLSLCode(const std::string& path,
                               const std::string& code,
                               int oglShaderType);

    /** data */
    bool m_autorelease;
    unsigned m_shader;
};

} // namespace GL
