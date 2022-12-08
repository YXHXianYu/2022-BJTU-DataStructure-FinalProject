#include "model.h"

#include <QOpenGLTexture>
#include <iostream>

namespace Hypercube {

Model::Model(QOpenGLFunctions_4_5_Core *gl_functions, const std::string &path) : gl_functions_(gl_functions) { LoadModel(path); }

void Model::Draw(QOpenGLShaderProgram &program) {
    for (unsigned int i = 0; i < meshes_.size(); i++) {
        meshes_[i].Draw(program);
    }
}

void Model::LoadModel(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSMIP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory_ = path.substr(0, path.find_last_of('/'));
    LoadNode(scene->mRootNode, scene);  // 开始递归遍历整个模型
}

void Model::LoadNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(LoadMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        LoadNode(node->mChildren[i], scene);
    }
}

Mesh Model::LoadMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    QVector3D ka, kd, ks, ke;

    // 处理顶点位置、法线和纹理坐标
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.position_ = vector;

        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.normal_ = vector;

        if (mesh->mTextureCoords[0])  // 是否存在纹理坐标
        {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.tex_coords_ = vec;
        } else
            vertex.tex_coords_ = QVector2D(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // 处理材质
    if (true /*mesh->mMaterialIndex >= 0*/) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);  // ambient
        ka = QVector3D(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);  // diffuse
        kd = QVector3D(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);  // specular
        ks = QVector3D(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, color);  // emissive
        ke = QVector3D(color.r, color.g, color.b);

        std::vector<Texture> diffuseMaps =
            LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");  // diffuse map
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps =
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");  // specular map
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(gl_functions_, vertices, indices, textures, ka, kd, ks, ke);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string type_name) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded_.size(); j++) {
            if (std::strcmp(textures_loaded_[j].path_.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded_[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id_ = LoadTextureFromFile(str.C_Str(), directory_);
            texture.type_ = type_name;
            texture.path_ = str.C_Str();
            textures.push_back(texture);
            textures_loaded_.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::LoadTextureFromFile(const std::string &file_name, const std::string &directory) {
    std::string path = directory + '/' + file_name;

    QOpenGLTexture *texture = new QOpenGLTexture(QImage(path.c_str()).mirrored());
    if (texture == NULL)
        qDebug() << "texture is NULL";
    else
        qDebug() << path.c_str() << "loaded";

    return texture->textureId();
}

}  // namespace Hypercube
