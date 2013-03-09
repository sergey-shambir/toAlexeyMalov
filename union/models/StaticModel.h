#pragma once
#include "ModelInterface.h"
#include "../math/math"
#include "../lights/TexturedMaterial.h"
#include "BoundingBox.h"
#include "VertexBuffer.h"
#include "StaticMesh.h"

// assimp types predeclaration
struct aiMesh;
struct aiScene;
struct aiMaterial;
struct aiMaterialProperty;
namespace Assimp { class Importer; }

namespace GL {

class StaticModel : public ModelInterface
{
    StaticModel();

public:
    void render(const RenderContext &context);
    const BoundingBox& boundingBox() const;

    static StaticModel *loadByPath(const std::string& path, const mat4 &transform);
    static void dumpInfoByPath(const std::string& path);


private:
    void loadMaterials(const aiScene& scene, const std::string &searchDir);
    void calculateBuffersSize(const aiScene& scene,
                              unsigned &szVerticies, unsigned &szIndicies);
    unsigned copyIndicies(const aiMesh& mesh, char* start);
    void loadMeshes(const aiScene& scene);

    static const aiScene *openScene(const std::string& path,
                                    Assimp::Importer& importer);

    static void dumpMatInfo(const aiMaterial& mat);
    static void dumpMatPropertyInfo(const aiMaterialProperty& prop);
    static void dumpMeshInfo(const aiMesh& mesh);

    static void copyMaterial(const aiMaterial& mat,
                             const std::string& textureSearchDir,
                             TexturedMaterial& outMat);

    mat4 m_transform;
    std::vector<TexturedMaterial> m_materials;
    std::vector<StaticMesh> m_meshes;
    BoundingBox m_box;

    VertexBuffer m_verticies;
    VertexBuffer m_indicies;

    int m_indiciesType; //! GL_UNSIGNED_SHORT or GL_UNSIGNED_INT
    bool m_hasTexCoords; //! default: false
};

} // namespace GL
