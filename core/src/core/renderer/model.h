#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "filesystem"
#include "mesh.h"
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <vector>

namespace renderer {

  struct Model {
  public:
    Model(const std::filesystem::path &path, bool should_flip);
    ~Model() = default;
    const std::filesystem::path &get_directory() { return m_directory; }
    void draw(const std::unique_ptr<Shader> &shader) const;

  private:
    void load_model_from_path(const std::filesystem::path &path);
    void process_node(aiNode *node, const aiScene *scene);
    void add_mesh_to_scene(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial *material, aiTextureType ai_type, TEXTURE_TYPE type);
    uint32_t TextureFromFile(const std::string &filename);
    std::filesystem::path m_directory;
    std::set<Texture, TextureLess> m_loaded_textures;
    std::vector<Mesh> m_meshes;
    bool m_should_flip;
  };
}
