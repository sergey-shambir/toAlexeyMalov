#include "Program.h"
#include <GL/glew.h>

#include <sstream>
#include <stdexcept>

namespace GL {

Program::Program()
    : m_autorelease(false)
    , m_program(0)
{
}

Program::~Program()
{
    if (m_autorelease)
        release();
}

Program Program::fromShaders1(const Shader &first)
{
    Program prog;
    prog.m_shaders.push_back(first.m_shader);
    prog.link();
    return prog;
}

Program Program::fromShaders2(const Shader &first, const Shader &second)
{
    Program prog;
    prog.m_shaders.push_back(first.m_shader);
    prog.m_shaders.push_back(second.m_shader);
    prog.link();
    return prog;
}

bool Program::canUse() const
{
    glValidateProgram(m_program);
    return getBoolParam(GL_VALIDATE_STATUS);
}

void Program::use() const
{
    glUseProgram(m_program);
}

int Program::getIntParam(int pname) const
{
    GLint value;
    glGetProgramiv(m_program, pname, &value);
    return value;
}

int Program::getBoolParam(int pname) const
{
    return GL_TRUE == getIntParam(pname);
}

int Program::getUniformLocation(const std::string &name) const
{
    auto it(m_uniformsCache.find(name));
    if (it != m_uniformsCache.end())
        return it->second;

    int ret = glGetUniformLocation(m_program, name.c_str());
    if (ret != -1)
        m_uniformsCache[name] = ret;
    return ret;
}

void Program::autorelease()
{
    m_autorelease = true;
}

std::string Program::getInfoLog() const
{
    GLint length = getIntParam(GL_INFO_LOG_LENGTH);
    if (0 == length)
        return std::string();

    std::string log;
    log.resize(length);
    glGetProgramInfoLog(m_program, length, &length, &log[0]);
    return log;
}

void Program::link()
{
    /* Initialize descriptors */
    m_program = glCreateProgram();
    for (unsigned shader : m_shaders)
        glAttachShader(m_program, shader);

    /* Link */
    glLinkProgram(m_program);
    if (!getBoolParam(GL_LINK_STATUS))
    {
        release();
        std::stringstream strm;
        strm << "Program " << m_program << " linkage failed:\n"
             << getInfoLog() << "\n";
        throw std::runtime_error(strm.str());
    }
}

void Program::release()
{
    if (0 != m_program)
        glDeleteProgram(m_program);
    for (unsigned shader : m_shaders)
        glDeleteShader(shader);
    m_shaders.clear();
}

} // namespace GL
