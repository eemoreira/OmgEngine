#pragma once

#include "shader.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace renderer {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
  };

  enum TEXTURE_TYPE {
    DIFFUSE,
    SPECULAR,
  };

  struct Texture {
    uint32_t id;
    TEXTURE_TYPE type;
    std::filesystem::path filename;
  };

  struct TextureLess {
    using is_transparent = void;

    bool operator()(const Texture &a, const Texture &b) const { return a.filename < b.filename; }

    bool operator()(const Texture &a, const std::filesystem::path &b) const { return a.filename < b; }

    bool operator()(const std::filesystem::path &a, const Texture &b) const { return a < b.filename; }
  };

  struct Mesh {
  public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<Texture> &textures);
    ~Mesh() = default;
    void setup();
    void draw(const std::unique_ptr<Shader> &shader) const;

    const std::vector<Vertex> &get_vertices() const { return m_vertices; }
    const std::vector<uint32_t> &get_indices() const { return m_indices; }
    const std::vector<Texture> &get_textures() const { return m_textures; }

  private:
    uint32_t vao, vbo, ebo;
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;
  };

}
