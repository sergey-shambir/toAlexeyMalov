#pragma once
#include <string>
#include "../math/math"

namespace GL {
class Program;
class PhongMaterial;
class TexturedMaterial;

/**
 * @class GL::ProgramBridge provides a way to load libunion classes as uniforms
 * See methods documentation in cpp file
 */
class ProgramBridge
{
    const Program &m_program;
public:
    /**
     * @brief Constructs instance with reference to program
     * @param program should be valid while you are calling any methods
     */
    ProgramBridge(const Program &program);

    void loadPhongMat(const PhongMaterial &mat, const std::string& structName);
    void loadTexturedMat(const TexturedMaterial &mat, const std::string& structName);

    void loadVec3(const vec3 &value, const std::string& name);
    void loadVec4(const vec4 &value, const std::string& name);
    void loadMat4(const mat4 &value, const std::string& name);
    void loadMat3(const mat3 &value, const std::string& name);
    void loadFloat(float value, const std::string& name);
    void loadInt(int value, const std::string& name);

private:
    ProgramBridge() = delete;
    ProgramBridge(const ProgramBridge &bridge) = delete;
    void operator =(const ProgramBridge &other) = delete;
};

} // namespace GL
