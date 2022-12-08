#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <string>
#include <vector>

namespace Hypercube {

struct Vertex {
    QVector3D position_;
    QVector3D normal_;
    QVector2D tex_coords_;
};

struct Texture {
    unsigned int id_;
    std::string type_;
    std::string path_;
};

/**
 * Mesh类
 * 储存一个模型的数据（顶点、索引、纹理）
 */
class Mesh {
   public:
    // 特殊函数重载（三）
    Mesh(QOpenGLFunctions_4_5_Core *gl_functions, std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures, QVector3D ka, QVector3D kd, QVector3D ks, QVector3D ke);  // constructor
    ~Mesh();                                                                                      // destructor
    Mesh(const Mesh &other);                                                                      // copy constructor (delete)
    Mesh &operator=(Mesh other);         // copy assignment operator (delete)
    friend void swap(Mesh &i, Mesh &j);  // swap

    // getter & setter
    QOpenGLFunctions_4_5_Core *gl_functions() const;
    void set_gl_functions(QOpenGLFunctions_4_5_Core *gl_functions);

    std::vector<Vertex> vertices() const;
    void set_vertices(const std::vector<Vertex> &vertices);

    std::vector<unsigned int> indices() const;
    void set_indices(const std::vector<unsigned int> &indices);

    std::vector<Texture> textures() const;
    void set_textures(const std::vector<Texture> &textures);

    // draw
    void Draw(QOpenGLShaderProgram &shader_program);

   private:
    void InitializeMesh();

   private:
    QOpenGLFunctions_4_5_Core *gl_functions_;  // opengl函数指针
    std::vector<Vertex> vertices_;             // 顶点
    std::vector<unsigned int> indices_;        // 索引
    std::vector<Texture> textures_;            // 纹理
    QVector3D ka_;                             // ambient color
    QVector3D kd_;                             // diffuse color
    QVector3D ks_;                             // specular color
    QVector3D ke_;                             // emissive color

    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
};

}  // namespace Hypercube

#endif  // MESH_H
