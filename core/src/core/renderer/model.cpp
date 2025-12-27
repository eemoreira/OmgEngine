#include "model.h"
#include "assimp/postprocess.h"
#include "core/logger.h"
#include <cstdint>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace renderer {

  Model::Model(const std::filesystem::path &path, bool should_flip) : m_should_flip(should_flip) {
    CORE_LOG_INFO("LOADING MODEL FROM {}", path.c_str());
    load_model_from_path(path);
    CORE_LOG_INFO("MODEL LOADED SUCCESSFULLY!");
  }

  void Model::load_model_from_path(const std::filesystem::path &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
      CORE_LOG_ERROR("ASSIMP:: {0}", importer.GetErrorString());
      return;
    }

    m_directory = path.parent_path();
    process_node(scene->mRootNode, scene);
  }

  void Model::process_node(aiNode *node, const aiScene *scene) {

    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      add_mesh_to_scene(mesh, scene);
    }

    for (int i = 0; i < node->mNumChildren; i++) process_node(node->mChildren[i], scene);
  }

  void Model::add_mesh_to_scene(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    for (int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      if (mesh->mTextureCoords[0])
        vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      vertices.emplace_back(vertex);
    }

    std::vector<uint32_t> indices;
    for (int i = 0; i < mesh->mNumFaces; i++)
      for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) indices.emplace_back(mesh->mFaces[i].mIndices[j]);

    std::vector<Texture> textures;
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_TYPE::DIFFUSE);
    for (int i = 0; i < mesh->mNumFaces; i++)
      for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) indices.emplace_back(mesh->mFaces[i].mIndices[j]);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    std::vector<Texture> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_TYPE::SPECULAR);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    m_meshes.emplace_back(vertices, indices, textures);
  }

  std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *material, aiTextureType ai_type, TEXTURE_TYPE type) {
    std::vector<Texture> textures;

    for (int i = 0; i < material->GetTextureCount(ai_type); i++) {
      aiString str;
      material->GetTexture(ai_type, i, &str);
      const std::string filename(str.C_Str());
      Texture tex;
      if (auto it = m_loaded_textures.find(filename); it != m_loaded_textures.end()) {
        tex = *it;
      } else {
        tex.id = TextureFromFile(filename);
        tex.type = type;
        tex.filename = filename;
        m_loaded_textures.insert(tex);
      }
      textures.emplace_back(tex);
    }

    return textures;
  }

  uint32_t Model::TextureFromFile(const std::string &filename) {
    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, channels;

    const std::filesystem::path &texture_path = m_directory / filename;
    stbi_set_flip_vertically_on_load(m_should_flip);
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);

    if (!data) {
      CORE_LOG_ERROR("TEXTURE::FAILED_TO_LOAD_TEXTURE_AT_PATH: {0}", texture_path.c_str());
      stbi_image_free(data);
      return tex;
    }

    if (channels == 1) {

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

    } else if (channels == 2) {

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

    } else if (channels == 3) {

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

    } else if (channels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    return tex;
  }

  void Model::draw(const std::unique_ptr<Shader> &shader) const {
    for (const Mesh &mesh : m_meshes) mesh.draw(shader);
  }

}
