#include "ProgramBridge.h"
#include "Program.h"
#include "../lights/PhongMaterial.h"
#include "../lights/TexturedMaterial.h"
#include "../math/math"
#include <GL/glew.h>

namespace GL {

ProgramBridge::ProgramBridge(const Program &program)
    : m_program(program)
{
}

/**
 * @brief ProgramBridge::loadPhongMat
 *
 * GLSL structure should look like this, where "myMaterial"
 * is valid structName value, and names of structure fields should be
 * exactly as in example:
 * @code
 *  struct PhongMaterial {
 *      vec4 ambient;
 *      vec4 diffuse;
 *      vec4 specular;
 *      float shininess;
 *  };
 *  uniform PhongMaterial myMaterial;
 * @endcode
 * @param mat - material value
 * @param structName - name of structure itself
 */
void ProgramBridge::loadPhongMat(const PhongMaterial &mat,
                                 const std::string &structName)
{
    loadVec4(mat.ambient(), structName + ".ambient");
    loadVec4(mat.diffuse(), structName + ".diffuse");
    loadVec4(mat.specular(), structName + ".specular");
    loadFloat(mat.shininess(), structName + ".shininess");
}

/**
 * @brief ProgramBridge::loadTexturedMat
 *
 * GLSL structure should look like this, where "myMaterial"
 * is valid structName value, and names of structure fields should be
 * exactly as in example:
 * @code
 *  struct PhongMaterial {
 *      vec4 ambient;
 *      vec4 diffuse;
 *      vec4 specular;
 *      float shininess;
 *  };
 *  uniform PhongMaterial myMaterial;
 * @endcode
 * @param mat - material value
 * @param structName - name of structure itself
 */
void ProgramBridge::loadTexturedMat(const TexturedMaterial &mat,
                                    const std::string &structName)
{
    loadPhongMat(mat, structName);
    // TODO: implement textures sampler loading
}

void ProgramBridge::loadVec3(const vec3 &value, const std::string &name)
{
    int location = m_program.getUniformLocation(name);
    glUniform4fv(location, 1, value);
}

void ProgramBridge::loadVec4(const vec4 &value, const std::string &name)
{
    int location = m_program.getUniformLocation(name);
    glUniform3fv(location, 1, value);
}

void ProgramBridge::loadMat4(const mat4 &value, const std::string &name)
{
    int location = m_program.getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void ProgramBridge::loadMat3(const mat3 &value, const std::string &name)
{
    int location = m_program.getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, value);
}

void ProgramBridge::loadFloat(float value, const std::string &name)
{
    int location = m_program.getUniformLocation(name);
    glUniform1f(location, value);
}

void ProgramBridge::loadInt(int value, const std::string &name)
{
    int location = m_program.getUniformLocation(name);
    glUniform1i(location, value);
}

} // namespace GL
