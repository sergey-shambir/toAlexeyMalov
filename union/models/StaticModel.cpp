#include "StaticModel.h"
#include "RenderContext.h"
#include "../lights/TexturedMaterial.h"
#include "../helpers/OpenGLHelper.h"

// we should check if file exists
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
#include <stdexcept>
#include <limits>

#include <stddef.h>
#include <stdio.h>

// assimp will be used for import model
#include <assimp/aiScene.h>
#include <assimp/assimp.hpp>
#include <assimp/aiPostProcess.h>

#include <GL/glew.h>

////////////////////////////////////////////////////////////////////////////////

inline static
std::string getParentDirectory(const std::string& filePath)
{
    size_t index = filePath.rfind('/');
    if (index == std::string::npos)
        return std::string();
    return filePath.substr(0, index + 1);
}

/**
 * @brief Recalculates min bound according vertex coords
 * @param vertex - new vertex that should be accounted
 * @param minBound - previous bound
 */
inline static
void fixMinBound(const aiVector3D& vertex, vec3 &minBound)
{
    minBound.x = std::min(minBound.x, vertex.x);
    minBound.y = std::min(minBound.y, vertex.y);
    minBound.z = std::min(minBound.z, vertex.z);
}

/**
 * @brief Recalculates max bound according vertex coords
 * @param vertex - new vertex that should be accounted
 * @param maxBound - previous bound
 */
inline static
void fixMaxBound(const aiVector3D& vertex, vec3 &maxBound)
{
    maxBound.x = std::max(maxBound.x, vertex.x);
    maxBound.y = std::max(maxBound.y, vertex.y);
    maxBound.z = std::max(maxBound.z, vertex.z);
}

inline static
GL::BoundingBox getBoundingBoxForNode(const aiScene& scene,
                                      const aiNode& node)
{
    vec3 min(+1e10f, +1e10f, +1e10f);
    vec3 max(-1e10f, -1e10f, -1e10f);

    for (unsigned mi = 0; mi < node.mNumMeshes; ++mi) {
        unsigned indexOnScene = node.mMeshes[mi];
        const aiMesh* mesh = scene.mMeshes[indexOnScene];
        for (unsigned vi = 0; vi < mesh->mNumVertices; ++vi) {
            struct aiVector3D vertex = mesh->mVertices[vi];
            fixMinBound(vertex, min);
            fixMaxBound(vertex, max);
        }
    }

    GL::BoundingBox box(min, max);
    for (unsigned ci = 0; ci < node.mNumChildren; ++ci) {
        const aiNode* child = node.mChildren[ci];
        GL::BoundingBox childBox(getBoundingBoxForNode(scene, *child));
        box.uniteWith(childBox);
    }

    return GL::BoundingBox(min, max);
}

inline static
mat4d getModelViewForNode(const aiNode& node)
{
    const float* data = node.mTransformation[0];
    mat4d ret;
    double* dest = &ret.a00;
    for (int i = 0; i < 16; ++i) {
        dest[i] = data[i];
    }
    return ret;
}

struct SimpleVertex {
    vec3 position;
    vec3 normal;
};

struct TexturedVertex {
    vec3 position;
    vec3 normal;
    vec2 diffuseUV;
};

inline static
void copyVec3to2(const aiVector3D& in, vec2 &out)
{
    out.x = in.x;
    out.y = in.y;
}

inline static
void copyColor(const aiColor3D& in, vec3 &out)
{
    out.x = in.r;
    out.y = in.g;
    out.z = in.b;
}

inline static
void copyVec3(const aiVector3D& in, vec3 &out)
{
    out.x = in.x;
    out.y = in.y;
    out.z = in.z;
}

inline static
int convertPrimitiveType(const unsigned aiValue)
{
    switch (aiValue) {
    default:
    case aiPrimitiveType_TRIANGLE:
        return GL_TRIANGLES;
    case aiPrimitiveType_LINE:
        return GL_LINES;
    case aiPrimitiveType_POINT:
        return GL_POINTS;
    }
}

template <class TVertex>
inline static
void copyVerticiesImpl(const aiMesh &mesh, char *start, const mat4& mat)
{
    TVertex* dest = reinterpret_cast<TVertex*>(start);
    for (unsigned vi = 0; vi < mesh.mNumVertices; ++vi, ++dest) {
        copyVec3(mesh.mVertices[vi], dest->position);
        copyVec3(mesh.mNormals[vi], dest->normal);
        dest->position = mat.transform3(dest->position);
        dest->normal = mat.transform3(dest->normal).normalized();
    }
}

inline static
void copyTexturedVerticies(const aiMesh &mesh, char *start, const mat4& mat)
{
    copyVerticiesImpl<TexturedVertex>(mesh, start, mat);

    TexturedVertex* dest = reinterpret_cast<TexturedVertex*>(start);
    if (mesh.HasTextureCoords(0)) {
        for (unsigned vi = 0; vi < mesh.mNumVertices; ++vi, ++dest) {
            copyVec3to2(mesh.mTextureCoords[0][vi], dest->diffuseUV);
        }
    } else {
        for (unsigned vi = 0; vi < mesh.mNumVertices; ++vi, ++dest) {
            dest->diffuseUV.x = 0;
            dest->diffuseUV.y = 0;
        }
    }
}

inline static
void copyVerticies(const aiMesh &mesh, char *start, const mat4& mat)
{
    copyVerticiesImpl<SimpleVertex>(mesh, start, mat);
}

template <class t_index> inline
unsigned copyIndiciesImpl(const aiMesh& mesh, char* start)
{
    if (mesh.mNumFaces == 0)
        return 0;

    t_index* dest = reinterpret_cast<t_index*>(start);
    for (unsigned fi = 0; fi < mesh.mNumFaces; ++fi) {
        const aiFace& face = mesh.mFaces[fi];
        for (unsigned ii = 0; ii < face.mNumIndices; ++ii) {
            *dest = face.mIndices[ii];
            ++dest;
        }
    }

    return mesh.mNumFaces * mesh.mFaces[0].mNumIndices;
}

////////////////////////////////////////////////////////////////////////////////

namespace GL {

StaticModel::StaticModel()
    : m_indiciesType(GL_UNSIGNED_INT)
    , m_hasTexCoords(false)
{
    m_indicies.setType(VertexBuffer::Type_Indicies);
}

void StaticModel::render(const RenderContext &context)
{
    glPushMatrix();
    mat4d modelView;
    OpenGLHelper::readModelViewMatrix(modelView);
    modelView = modelView * context.m_transform;
    OpenGLHelper::setModelViewMatrix(modelView);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if (m_hasTexCoords)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    m_verticies.bind();
    m_indicies.bind();

    if (m_hasTexCoords) {
        const size_t offsetPos = offsetof(TexturedVertex, position);
        const size_t offsetNorm = offsetof(TexturedVertex, normal);
        const size_t offsetTex = offsetof(TexturedVertex, diffuseUV);
        for (StaticMesh& mesh : m_meshes) {
            const TexturedMaterial& mat = m_materials[mesh.m_materialIndex];
            mat.install();

            const size_t offsetMesh = mesh.m_rangeVert.m_start;
            const size_t indiciesStart = mesh.m_rangeInd.m_start;
            const void* pVerticies = (const void*)(offsetMesh + offsetPos);
            const void* pNormals = (const void*)(offsetMesh + offsetNorm);
            const void* pTextures = (const void*)(offsetMesh + offsetTex);
            const void* pIndicies = (const void*)(indiciesStart);

            const size_t szIndex = (m_indiciesType == GL_UNSIGNED_SHORT)
                    ? sizeof(unsigned short) : sizeof(unsigned int);
            const size_t numIndicies = mesh.m_rangeInd.m_length / szIndex;

            glVertexPointer(3, GL_FLOAT, sizeof(TexturedVertex), pVerticies);
            glNormalPointer(GL_FLOAT, sizeof(TexturedVertex), pNormals);
            glTexCoordPointer(2, GL_FLOAT, sizeof(TexturedVertex), pTextures);
            glDrawElements(mesh.m_glPrimitiveType, numIndicies,
                           m_indiciesType, pIndicies);
        }
    } else {
        const size_t offsetPos = offsetof(SimpleVertex, position);
        const size_t offsetNorm = offsetof(SimpleVertex, normal);
        for (StaticMesh& mesh : m_meshes) {
            const TexturedMaterial& mat = m_materials[mesh.m_materialIndex];
            mat.install();

            const size_t offsetMesh = mesh.m_rangeVert.m_start;
            const size_t indiciesStart = mesh.m_rangeInd.m_start;
            const void* pVerticies = (const void*)(offsetMesh + offsetPos);
            const void* pNormals = (const void*)(offsetMesh + offsetNorm);
            const void* pIndicies = (const void*)(indiciesStart);

            const size_t szIndex = (m_indiciesType == GL_UNSIGNED_SHORT)
                    ? sizeof(unsigned short) : sizeof(unsigned int);
            const size_t numIndicies = mesh.m_rangeInd.m_length / szIndex;

            glVertexPointer(3, GL_FLOAT, sizeof(SimpleVertex), pVerticies);
            glNormalPointer(GL_FLOAT, sizeof(SimpleVertex), pNormals);
            glDrawElements(mesh.m_glPrimitiveType, numIndicies,
                           m_indiciesType, pIndicies);
        }
    }

    m_verticies.unbind();
    m_indicies.unbind();

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (m_hasTexCoords)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}

const BoundingBox &StaticModel::boundingBox() const
{
    return m_box;
}

/**
 * @brief Loads model file places at given path
 *
 * See assimp library version 2 documentation if you want know
 * all supported formats.
 *
 * @param path - native for current OS path to file
 * @return always non-null pointer to new instance
 * @throw std::runtime_error if loading failed
 */
StaticModel *StaticModel::loadByPath(const std::string &path, const mat4 &transform)
{
    Assimp::Importer importer;
    const aiScene *scene = openScene(path, importer);
    std::string searchDir = getParentDirectory(path); /* resources search dir */

    std::unique_ptr<StaticModel> pModel(new StaticModel);
    pModel->m_box = getBoundingBoxForNode(*scene, *(scene->mRootNode));
    pModel->m_transform = transform;
    pModel->loadMaterials(*scene, searchDir);
    pModel->loadMeshes(*scene);

    return pModel.release();
}

void StaticModel::dumpInfoByPath(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = openScene(path, importer);

    printf("Model has");
    if (scene->mNumAnimations != 0)
        printf(" %u animations", scene->mNumAnimations);
    if (scene->mNumCameras != 0)
        printf(" %u cameras", scene->mNumCameras);
    if (scene->mNumLights != 0)
        printf(" %u lights", scene->mNumLights);
    if (scene->mNumMaterials != 0)
        printf(" %u materials", scene->mNumMaterials);
    if (scene->mNumMeshes != 0)
        printf(" %u meshes", scene->mNumMeshes);
    if (scene->mNumTextures != 0)
        printf(" %u standalone textures", scene->mNumTextures);
    putchar('\n');

    for (unsigned mi = 0; mi < scene->mNumMaterials; ++mi) {
        const aiMaterial* mat = scene->mMaterials[mi];
        dumpMatInfo(*mat);
    }

    for (unsigned mi = 0; mi < scene->mNumMeshes; ++mi) {
        const aiMesh& mesh = *(scene->mMeshes[mi]);
        dumpMeshInfo(mesh);
    }

    fflush(stdout);
}

////////////////////////////////////////////////////////////////////////////////

void StaticModel::loadMaterials(const aiScene &scene, const std::string& searchDir)
{
    m_materials.resize(scene.mNumMaterials);
    for (unsigned mi = 0; mi < scene.mNumMaterials; ++mi) {
        const aiMaterial& mat = *(scene.mMaterials[mi]);
        TexturedMaterial& outMat = m_materials[mi];
        copyMaterial(mat, searchDir, outMat);
    }
}

/**
 * @brief Calculates buffers size in bytes
 * Also sets m_indiciesType and m_hasTexCoords
 * @param szVerticies - [out] size of resulting verticies array in bytes
 * @param szIndicies - [out] size of resulting indicies array in bytes
 */
void StaticModel::calculateBuffersSize(const aiScene& scene,
                                       unsigned &szVerticies, unsigned &szIndicies)
{
    unsigned PACK_LIMIT(std::numeric_limits<unsigned short>::max());
    unsigned numIndicies(0);
    unsigned numVerticies(0);
    bool hasTexCoords = false;
    for (unsigned mi = 0; mi < scene.mNumMeshes; ++mi) {
        const aiMesh& mesh = *(scene.mMeshes[mi]);
        if (mesh.mNumFaces > 0) {
            unsigned indiciesPerFace(mesh.mFaces[0].mNumIndices);
            numIndicies += indiciesPerFace * mesh.mNumFaces;
        }
        numVerticies += mesh.mNumVertices;
        hasTexCoords = hasTexCoords || mesh.HasTextureCoords(0);
    }

    m_hasTexCoords = hasTexCoords;
    if (hasTexCoords)
        szVerticies = sizeof(TexturedVertex) * numVerticies;
    else
        szVerticies = sizeof(SimpleVertex) * numVerticies;

    /* Use 16-bit indicies if possible, 32-bit otherwise */
    if (numVerticies >= PACK_LIMIT) {
        szIndicies = numIndicies * sizeof(unsigned int);
        m_indiciesType = GL_UNSIGNED_INT;
    } else {
        szIndicies = numIndicies * sizeof(unsigned short);
        m_indiciesType = GL_UNSIGNED_SHORT;
    }
}

/**
 * @brief Copies indicies to prepared buffer
 * @param mesh - assimp mesh
 * @param start - pointer to buffer start
 * @return amount of indicies copied
 */
unsigned StaticModel::copyIndicies(const aiMesh& mesh, char* start)
{
    if (m_indiciesType == GL_UNSIGNED_SHORT) {
        return copyIndiciesImpl<unsigned short>(mesh, start);
    } else {
        return copyIndiciesImpl<unsigned int>(mesh, start);
    }
}

/**
 * @brief Loads meshes to internal buffers
 * @param scene - assimp scene
 */
void StaticModel::loadMeshes(const aiScene &scene)
{
    unsigned szVerticies(0);
    unsigned szIndicies(0);
    calculateBuffersSize(scene, szVerticies, szIndicies);

    /* delete before exit */
    char* const pVerticies(new char[szVerticies + szIndicies]);
    char* const pIndicies = pVerticies + szVerticies;
    char* pMeshVert = pVerticies;
    char* pMeshInd = pIndicies;

    unsigned szMultiplier = sizeof(unsigned short);
    if (m_indiciesType != GL_UNSIGNED_SHORT) {
        szMultiplier = sizeof(unsigned int);
    }

    m_meshes.resize(scene.mNumMeshes);
    for (unsigned mi = 0; mi < scene.mNumMeshes; ++mi) {
        const aiMesh& mesh = *(scene.mMeshes[mi]);

        StaticMesh& outMesh = m_meshes[mi];
        outMesh.m_materialIndex = mesh.mMaterialIndex;
        outMesh.m_glPrimitiveType = convertPrimitiveType(mesh.mPrimitiveTypes);

        unsigned szAddenum = 0;
        if (m_hasTexCoords) {
            copyTexturedVerticies(mesh, pMeshVert, m_transform);
            szAddenum = mesh.mNumVertices * sizeof(TexturedVertex);
        } else {
            copyVerticies(mesh, pMeshVert, m_transform);
            szAddenum = mesh.mNumVertices * sizeof(SimpleVertex);
        }

        outMesh.m_rangeVert.m_start = pMeshVert - pVerticies;
        outMesh.m_rangeVert.m_length = szAddenum;
        pMeshVert += szAddenum;

        szAddenum = szMultiplier * copyIndicies(mesh, pMeshInd);
        outMesh.m_rangeInd.m_start = pMeshInd - pIndicies;
        outMesh.m_rangeInd.m_length = szAddenum;
        pMeshInd += szAddenum;
    }

    m_verticies.bind();
    m_verticies.init(pVerticies, szVerticies);
    m_verticies.unbind();

    m_indicies.bind();
    m_indicies.init(pIndicies, szIndicies);
    m_indicies.unbind();

    delete[] pVerticies;
}

const aiScene *StaticModel::openScene(const std::string &path,
                                      Assimp::Importer &importer)
{
    if (0 != access(path.c_str(), R_OK)) {
        throw std::runtime_error("File not exists: " + path);
    }

    unsigned importFlags = aiProcessPreset_TargetRealtime_Quality;
    importFlags &= ~aiProcess_CalcTangentSpace;

    /* importer destructor cleans scene */
    const aiScene *scene = importer.ReadFile(path, importFlags);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    for (unsigned mi = 0; mi < scene->mNumMeshes; ++mi) {
        const aiMesh& mesh = *(scene->mMeshes[mi]);
        if (!mesh.HasNormals())
            throw std::runtime_error("at least one of model meshes has no normals");
        if (mesh.mPrimitiveTypes == aiPrimitiveType_POLYGON)
            throw std::runtime_error("at least one of model meshes consists of "
                                     "polygons instead of triangles, lines and points");
    }

    return scene;
}

void StaticModel::dumpMatInfo(const aiMaterial &mat)
{
    printf("--------------------------------------------\n"
           "Material has: %u diffuse textures, "
           "%u ambient textures, %u specular textures, "
           "%u emissive textures, %u height maps, "
           "%u normal maps, %u shininess textures, "
           "%u lightmaps\n",
           mat.GetTextureCount(aiTextureType_DIFFUSE),
           mat.GetTextureCount(aiTextureType_AMBIENT),
           mat.GetTextureCount(aiTextureType_SPECULAR),
           mat.GetTextureCount(aiTextureType_EMISSIVE),
           mat.GetTextureCount(aiTextureType_HEIGHT),
           mat.GetTextureCount(aiTextureType_NORMALS),
           mat.GetTextureCount(aiTextureType_SHININESS),
           mat.GetTextureCount(aiTextureType_LIGHTMAP));
    if (0 != mat.mNumProperties) {
        printf("It also has %u properties and allocates %u space\n",
               mat.mNumProperties, mat.mNumAllocated);
        for (unsigned pi = 0; pi < mat.mNumProperties; ++pi) {
            const aiMaterialProperty* property = mat.mProperties[pi];
            dumpMatPropertyInfo(*property);
        }
    }

    for (int ti = 0; ; ++ti) {
        aiString path;
        if (AI_SUCCESS != mat.GetTexture(aiTextureType_DIFFUSE, ti, &path)) {
            break;
        }
        printf("Texture %d path %s\n", ti, path.data);
    }
    printf("--------------------------------------------\n");
}

void StaticModel::dumpMatPropertyInfo(const aiMaterialProperty &prop)
{
    printf("Property %s, value", prop.mKey.data);
    switch (prop.mType) {
    case aiPTI_Float:
        for(unsigned i = 0; i < prop.mDataLength; i += sizeof(float))
            printf(" %f", *reinterpret_cast<float*>(prop.mData + i));
        break;

    case aiPTI_Integer:
        for(unsigned i = 0; i < prop.mDataLength; i += sizeof(int))
            printf(" %d", *reinterpret_cast<int*>(prop.mData + i));
        break;

    case aiPTI_Buffer:
        printf(" <some buffer>");
        break;

    case aiPTI_String:
        printf(" <some string>");
        break;

    default:
        printf(" unknown <unknown type>");
        break;
    }
    putchar('\n');
}

void StaticModel::dumpMeshInfo(const aiMesh &mesh)
{
    printf("Mesh attributes string: ");
    if (mesh.HasBones())
        printf("b");
    if (mesh.HasFaces())
        printf("i");
    if (mesh.HasNormals())
        printf("n");
    if (mesh.HasPositions())
        printf("v");
    if (mesh.HasTangentsAndBitangents())
        printf("t");
    if (mesh.HasTextureCoords(0))
        printf("s");
    if (mesh.HasVertexColors(0))
        printf("c");
    putchar('\n');

    if (mesh.mNumBones > 0) {
        printf("Mesh has %d bones\n", mesh.mNumBones);
        for (unsigned i = 0; i < mesh.mNumBones; ++i) {
            const aiBone& bone = *(mesh.mBones[i]);
            printf("Bone %s with %d weights\n", bone.mName.data, bone.mNumWeights);
        }
    }
}

void StaticModel::copyMaterial(const aiMaterial &mat,
                               const std::string& textureSearchDir,
                               TexturedMaterial &outMat)
{
    aiColor3D color(0, 0, 0);
    vec3 buffer;
    if (AI_SUCCESS == mat.Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
        copyColor(color, buffer);
        outMat.setDiffuse(buffer.clamped());
    }
    if (AI_SUCCESS == mat.Get(AI_MATKEY_COLOR_AMBIENT, color)) {
        copyColor(color, buffer);
        outMat.setAmbient(buffer.clamped());
    }
    if (AI_SUCCESS == mat.Get(AI_MATKEY_COLOR_SPECULAR, color)) {
        copyColor(color, buffer);
        outMat.setSpecular(buffer.clamped());
    }

    float shininess = 0;
    if (AI_SUCCESS == mat.Get(AI_MATKEY_SHININESS, shininess))
        outMat.setShininess(shininess);

    aiString path;
    if (AI_SUCCESS == mat.GetTexture(aiTextureType_DIFFUSE, 0, &path))
    {
        try {
            std::string realPath = textureSearchDir + path.data;
            Texture2D texture(Texture2D::createFromImageByPath(realPath));
            outMat.setDiffuseTexture(texture);
        } catch (const std::runtime_error& loadingException) {
            printf("%s\n", loadingException.what());
            fflush(stdout);
        }
    }
}

} // namespace GL
