#include "mesh.h"

namespace Hypercube {

Mesh::Mesh(QOpenGLFunctions_4_5_Core *gl_functions, std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures, QVector3D ka, QVector3D kd, QVector3D ks, QVector3D ke)
    : gl_functions_(gl_functions),
      vertices_(vertices),
      indices_(indices),
      textures_(textures),
      ka_(ka),
      kd_(kd),
      ks_(ks),
      ke_(ke) {
    InitializeMesh();
}

Mesh::~Mesh() {
    gl_functions_->glDeleteVertexArrays(1, &vao_);
    gl_functions_->glDeleteBuffers(1, &vbo_);
    gl_functions_->glDeleteBuffers(1, &ebo_);
}

Mesh::Mesh(const Mesh &other)
    : gl_functions_(other.gl_functions_),
      vertices_(other.vertices_),
      indices_(other.indices_),
      textures_(other.textures_),
      ka_(other.ka_),
      kd_(other.kd_),
      ks_(other.ks_),
      ke_(other.ke_) {
    InitializeMesh();
}

Mesh &Mesh::operator=(Mesh other) {
    swap(*this, other);
    return *this;
}

void swap(Mesh &i, Mesh &j) {  // 友元函数
    using std::swap;
    swap(i.vao_, j.vao_);
    swap(i.vbo_, j.vbo_);
    swap(i.ebo_, j.ebo_);
    swap(i.gl_functions_, j.gl_functions_);
    swap(i.vertices_, j.vertices_);
    swap(i.indices_, j.indices_);
    swap(i.textures_, j.textures_);
    swap(i.ka_, j.ka_);
    swap(i.kd_, j.kd_);
    swap(i.ks_, j.ks_);
    swap(i.ke_, j.ke_);
}

QOpenGLFunctions_4_5_Core *Mesh::gl_functions() const { return gl_functions_; }

void Mesh::set_gl_functions(QOpenGLFunctions_4_5_Core *gl_functions) { gl_functions_ = gl_functions; }

std::vector<Vertex> Mesh::vertices() const { return vertices_; }

void Mesh::set_vertices(const std::vector<Vertex> &vertices) { vertices_ = vertices; }

std::vector<unsigned int> Mesh::indices() const { return indices_; }

void Mesh::set_indices(const std::vector<unsigned int> &indices) { indices_ = indices; }

std::vector<Texture> Mesh::textures() const { return textures_; }

void Mesh::set_textures(const std::vector<Texture> &textures) { textures_ = textures; }

void Mesh::Draw(QOpenGLShaderProgram &shader_program) {
    unsigned int diffuse_cnt = 0;
    unsigned int specular_cnt = 0;
    // material
    shader_program.setUniformValue("material.have_texture", bool(textures_.size() > 0));
    for (unsigned int i = 0; i < textures_.size(); i++) {
        gl_functions_->glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures_[i].type_;
        if (name == "texture_diffuse")
            number = std::to_string(++diffuse_cnt);
        else if (name == "texture_specular")
            number = std::to_string(++specular_cnt);

        shader_program.setUniformValue(("material." + name + number).c_str(), i);  // 连接Mesh与Shader
        gl_functions_->glBindTexture(GL_TEXTURE_2D, textures_[i].id_);             // 连接QtOpenGL(CPU)与Mesh
    }
    shader_program.setUniformValue("material.ka", ka_.x(), ka_.y(), ka_.z());
    shader_program.setUniformValue("material.kd", kd_.x(), kd_.y(), kd_.z());
    shader_program.setUniformValue("material.ks", ks_.x(), ks_.y(), ks_.z());
    shader_program.setUniformValue("material.ke", ke_.x(), ke_.y(), ke_.z());

    gl_functions_->glBindVertexArray(vao_);
    // gl_functions_->glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    gl_functions_->glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::InitializeMesh() {
    gl_functions_->glGenVertexArrays(1, &vao_);
    gl_functions_->glGenBuffers(1, &vbo_);
    gl_functions_->glGenBuffers(1, &ebo_);

    gl_functions_->glBindVertexArray(vao_);
    gl_functions_->glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    gl_functions_->glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
    gl_functions_->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    gl_functions_->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    gl_functions_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position_));
    gl_functions_->glEnableVertexAttribArray(0);
    gl_functions_->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal_));
    gl_functions_->glEnableVertexAttribArray(1);
    gl_functions_->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords_));
    gl_functions_->glEnableVertexAttribArray(2);
}

// void Mesh::initializeMesh() {
//     // vao
//     gl_functions_->glGenVertexArrays(1, &vao_);
//     gl_functions_->glBindVertexArray(vao_);

//    // vbo
//    gl_functions_->glGenBuffers(1, &vbo_);
//    gl_functions_->glBindBuffer(GL_ARRAY_BUFFER, vbo_);
//    gl_functions_->glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

//    // ebo
//    gl_functions_->glGenBuffers(1, &ebo_);
//    gl_functions_->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
//    gl_functions_->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

//    // resolve
//    gl_functions_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position_));
//    gl_functions_->glEnableVertexAttribArray(0);
//    gl_functions_->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal_));
//    gl_functions_->glEnableVertexAttribArray(1);
//    gl_functions_->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords_));
//    gl_functions_->glEnableVertexAttribArray(2);
//}
}  // namespace Hypercube
