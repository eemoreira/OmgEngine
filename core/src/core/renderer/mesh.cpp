#include "mesh.h"
#include "core/logger.h"
#include <glad/gl.h>

namespace renderer {

  Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<Texture> &textures)
      : m_textures(textures), m_indices(indices), m_vertices(vertices) {
    setup();
  }

  void Mesh::setup() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    const uint32_t stride = sizeof(Vertex);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
  }

  void Mesh::draw(const std::unique_ptr<Shader> &shader) const {
    shader->use();
     uint32_t diffuse_cnt = 1, specular_cnt = 1;
     for (int i = 0; i < int(m_textures.size()); i++) {
       const Texture &texture = m_textures[i];
       glActiveTexture(GL_TEXTURE0 + i);

      std::string number, type_name;
      if (texture.type == DIFFUSE) {
        number = std::to_string(diffuse_cnt++);
        type_name = "diffuse";
      } else if (texture.type == SPECULAR) {
        number = std::to_string(specular_cnt++);
        type_name = "specular";
      }

      shader->setInt("material." + type_name + number, i);

      glBindTexture(GL_TEXTURE_2D, texture.id);
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, int(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
     for (int i = 0; i < int(m_textures.size()); i++) {
       glActiveTexture(GL_TEXTURE0 + i);
       glBindTexture(GL_TEXTURE_2D, 0);
     }
  }

}
