#include "Shader.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdexcept>
#include <sstream>

namespace GL {

Shader::Shader()
    : m_autorelease(false)
    , m_shader(0)
{
}

Shader::~Shader()
{
    if (m_autorelease)
        glDeleteShader(m_shader);
}

Shader Shader::createFromFile(const std::string &path, int oglShaderType)
{
    FILE *file(fopen(path.c_str(), "r"));
    if (!file) {
        throw std::runtime_error("Cannot open shader by path " + path);
    }
    std::string content;
    int readnum(0);
    do {
        char buffer[4096];
        readnum = fread(buffer, sizeof(char), 4096, file);
        content.append(buffer, readnum);
    } while (0 < readnum);

    return fromGLSLCode(path, content, oglShaderType);
}

Shader Shader::createFromAscii(const std::string &code, int oglShaderType)
{
    return fromGLSLCode("<string>", code, oglShaderType);
}

Shader Shader::fromGLSLCode(const std::string &path, const std::string &code, int oglShaderType)
{
    /* split lines */
    std::vector<const GLchar*> lines;
    std::vector<GLint> lengths;
    size_t index = 0;
    for (size_t from(0); from < code.length(); from = index + 1) {
        index = code.find('\n', from);
        if (index == std::string::npos)
            index = code.length();
        const char *line = code.c_str() + from;
        lines.push_back(reinterpret_cast<const GLchar*>(line));
        lengths.push_back(index - from);
    }
    const GLsizei count = static_cast<GLsizei>(lines.size());

    /* interact with OpenGL */
    Shader ret;
    ret.m_shader = glCreateShader(oglShaderType);
    glShaderSource(ret.m_shader, count, lines.data(), lengths.data());
    glCompileShader(ret.m_shader);

    /* check compilation status */
    if (!ret.getBoolParameter(GL_COMPILE_STATUS))
    {
        ret.autorelease();
        std::stringstream strm;
        strm << "Shader " << path << " compilation failed:\n"
             << ret.getInfoLog() << "\n";
        throw std::runtime_error(strm.str());
    }

    return ret;
}

void Shader::autorelease()
{
    m_autorelease = true;
}

int Shader::getIntParameter(int pname) const
{
    GLint value;
    glGetShaderiv(m_shader, pname, &value);
    return value;
}

bool Shader::getBoolParameter(int pname) const
{
    return GL_TRUE == getIntParameter(pname);
}

std::string Shader::getInfoLog() const
{
    GLint length = getIntParameter(GL_INFO_LOG_LENGTH);
    if (0 == length)
        return std::string();

    std::string log;
    log.resize(length);
    glGetShaderInfoLog(m_shader, length, &length, &log[0]);
    return log;
}

} // namespace GL
