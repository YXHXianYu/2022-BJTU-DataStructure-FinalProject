#ifndef MODEL_H
#define MODEL_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "mesh.h"

namespace Hypercube {

class Model {
   public:
    Model(QOpenGLFunctions_4_5_Core *gl_functions, const std::string &path);

    void Draw(QOpenGLShaderProgram &program);

   private:
    QOpenGLFunctions_4_5_Core *gl_functions_;  // opengl函数指针
    std::vector<Texture> textures_loaded_;
    std::vector<Mesh> meshes_;
    std::string directory_;

    void LoadModel(const std::string &path);            // 根据目录加载模型
    void LoadNode(aiNode *node, const aiScene *scene);  // 递归处理模型的所有节点
    Mesh LoadMesh(aiMesh *mesh, const aiScene *scene);  // 把aiMesh处理为Mesh
    std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string type_name);
    unsigned int LoadTextureFromFile(const std::string &path, const std::string &directory);
};
}  // namespace Hypercube

#endif  // MODEL_H
