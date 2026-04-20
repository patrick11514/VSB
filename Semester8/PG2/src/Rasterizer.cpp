#include "Rasterizer.hpp"
#include "Camera.hpp"
#include "Controller.hpp"
#include "Shader/ShaderProgram.hpp"
#include "attributes.hpp"
#include "system/CameraSyncSystem.hpp"
#include "system/RenderSystem.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <soil2/SOIL2.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

namespace
{
  struct EdgeKey
  {
    unsigned int a;
    unsigned int b;

    bool operator==(const EdgeKey &other) const
    {
      return a == other.a && b == other.b;
    }
  };

  struct EdgeKeyHash
  {
    size_t operator()(const EdgeKey &key) const
    {
      return (static_cast<size_t>(key.a) << 32) ^ static_cast<size_t>(key.b);
    }
  };

  std::vector<unsigned int>
  BuildAdjacencyIndices(const std::vector<unsigned int> &indices)
  {
    std::unordered_map<EdgeKey, unsigned int, EdgeKeyHash> opposite;
    opposite.reserve(indices.size());

    for (size_t i = 0; i + 2 < indices.size(); i += 3)
    {
      const unsigned int a = indices[i + 0];
      const unsigned int b = indices[i + 1];
      const unsigned int c = indices[i + 2];
      opposite[{a, b}] = c;
      opposite[{b, c}] = a;
      opposite[{c, a}] = b;
    }

    std::vector<unsigned int> adjacency;
    adjacency.reserve((indices.size() / 3) * 6);

    for (size_t i = 0; i + 2 < indices.size(); i += 3)
    {
      const unsigned int a = indices[i + 0];
      const unsigned int b = indices[i + 1];
      const unsigned int c = indices[i + 2];

      const auto it0 = opposite.find({b, a});
      const auto it1 = opposite.find({c, b});
      const auto it2 = opposite.find({a, c});

      const unsigned int adj0 = (it0 != opposite.end()) ? it0->second : c;
      const unsigned int adj1 = (it1 != opposite.end()) ? it1->second : a;
      const unsigned int adj2 = (it2 != opposite.end()) ? it2->second : b;

      adjacency.push_back(a);
      adjacency.push_back(adj0);
      adjacency.push_back(b);
      adjacency.push_back(adj1);
      adjacency.push_back(c);
      adjacency.push_back(adj2);
    }

    return adjacency;
  }
} // namespace

Rasterizer::Rasterizer(int width, int height, const char *title)
    : width(width), height(height), title(title), window(nullptr),
      camera(nullptr), controller(nullptr), program(nullptr),
      depthProgram(nullptr), shadowVolumeProgram(nullptr),
      shadowDarkenProgram(nullptr), stencilDebugProgram(nullptr),
      materialSSBO(0) {}

Rasterizer::~Rasterizer()
{
  delete camera;
  delete program;
  delete depthProgram;
  delete shadowVolumeProgram;
  delete shadowDarkenProgram;
  delete stencilDebugProgram;

  if (shadowDepthMap != 0)
  {
    glDeleteTextures(1, &shadowDepthMap);
  }
  if (shadowFBO != 0)
  {
    glDeleteFramebuffers(1, &shadowFBO);
  }
  if (fullscreenVao != 0)
  {
    glDeleteVertexArrays(1, &fullscreenVao);
  }
  if (offscreenColorTex != 0)
  {
    glDeleteTextures(1, &offscreenColorTex);
  }
  if (offscreenDepthStencilRbo != 0)
  {
    glDeleteRenderbuffers(1, &offscreenDepthStencilRbo);
  }
  if (offscreenFbo != 0)
  {
    glDeleteFramebuffers(1, &offscreenFbo);
  }

  for (auto &mesh : scene.meshes)
  {
    if (mesh.adjacencyEbo != 0)
    {
      glDeleteBuffers(1, &mesh.adjacencyEbo);
      mesh.adjacencyEbo = 0;
    }
  }
  // Window deletion handled by glfwTerminate usually
  if (window)
  {
    glfwDestroyWindow(window);
  }
}

void Rasterizer::resize(int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, width, height);
  RecreateOffscreenFramebuffer();
}

void Rasterizer::RecreateOffscreenFramebuffer()
{
  if (offscreenColorTex != 0)
  {
    glDeleteTextures(1, &offscreenColorTex);
    offscreenColorTex = 0;
  }
  if (offscreenDepthStencilRbo != 0)
  {
    glDeleteRenderbuffers(1, &offscreenDepthStencilRbo);
    offscreenDepthStencilRbo = 0;
  }
  if (offscreenFbo != 0)
  {
    glDeleteFramebuffers(1, &offscreenFbo);
    offscreenFbo = 0;
  }

  glGenFramebuffers(1, &offscreenFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, offscreenFbo);

  glGenTextures(1, &offscreenColorTex);
  glBindTexture(GL_TEXTURE_2D, offscreenColorTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         offscreenColorTex, 0);

  glGenRenderbuffers(1, &offscreenDepthStencilRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, offscreenDepthStencilRbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, offscreenDepthStencilRbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    throw std::runtime_error("ERROR: Offscreen framebuffer for stencil fallback is incomplete");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Rasterizer::error_callback([[maybe_unused]] int error,
                                const char *description)
{
  std::cerr << "GLFW Error: " << description << std::endl;
}

void Rasterizer::InitDevice()
{
  glfwSetErrorCallback(error_callback);
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  if (!glfwInit())
  {
    throw std::runtime_error("ERROR: could not start GLFW3");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    throw std::runtime_error("ERROR: could not create GLFW3 window");
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
    printf("GLEW Error: %s\n", glewGetErrorString(err));
  if (err != GLEW_OK)
  {
    throw std::runtime_error("ERROR: could not init GLEW");
  }

  glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
  activeStencilBits = stencilBits;
  RecreateOffscreenFramebuffer();

  // Set up viewport and camera (Using defaults here instead of full scene
  // configuration for now) You should integrate proper Camera parameters here,
  // per your original app.
  camera = new Camera(60.f, 0.1f, 1000.f);
  controller = Controller::getInstance(this, camera);

  // We actually need *both* in callbacks, so let's pass the Rasterizer as the
  // root, and we can access Controller from it.
  glfwSetWindowUserPointer(window, this);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                int action, int mods)
                     {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    Rasterizer *rast =
        static_cast<Rasterizer *>(glfwGetWindowUserPointer(window));
    Controller *ctrl = rast->controller;

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
      rast->uiMode = !rast->uiMode;
      if (rast->uiMode) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
    }

    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
      rast->showAxes = !rast->showAxes;
      auto view =
          rast->registry.view<attributes::IsAxis, attributes::Togglable>();
      view.each([&](entt::entity, attributes::Togglable &tog) {
        tog.visible = rast->showAxes;
      });
    }

    if (!rast->uiMode) {
      ctrl->onKeyPress(window, key, scancode, action, mods);
    } });

  glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y)
                           {
    Rasterizer *rast =
        static_cast<Rasterizer *>(glfwGetWindowUserPointer(window));
    Controller *ctrl = rast->controller;
    if (!rast->uiMode) {
      ctrl->onMouse(window, x, y);
    } });

  glfwSetScrollCallback(
      window, [](GLFWwindow *window, double xoffset, double yoffset)
      {
        Rasterizer *rast =
            static_cast<Rasterizer *>(glfwGetWindowUserPointer(window));
        if (!rast->uiMode) {
          rast->controller->cameraSpeed += static_cast<float>(yoffset) * 0.05f;
          if (rast->controller->cameraSpeed < 0.01f) {
            rast->controller->cameraSpeed = 0.01f;
          }
        } });

  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *window, int width, int height)
      {
        Rasterizer *rast =
            static_cast<Rasterizer *>(glfwGetWindowUserPointer(window));
        if (rast)
          rast->resize(width, height); });

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 430");
}

void Rasterizer::InitPrograms()
{
  program = new ShaderProgram("../shaders/vertex/Base.vert",
                              "../shaders/fragment/BasePBR.frag", controller);
  depthProgram = new ShaderProgram("../shaders/vertex/DepthOnly.vert",
                                   "../shaders/fragment/DepthOnly.frag");
  shadowVolumeProgram = new ShaderProgram(
      "../shaders/vertex/ShadowVolume.vert",
      "../shaders/geometry/ShadowVolume.geom",
      "../shaders/fragment/ShadowVolume.frag");
  shadowDarkenProgram = new ShaderProgram("../shaders/vertex/ShadowDarken.vert",
                                          "../shaders/fragment/ShadowDarken.frag");
  stencilDebugProgram = new ShaderProgram("../shaders/vertex/ShadowDarken.vert",
                                          "../shaders/fragment/StencilDebug.frag");

  glGenVertexArrays(1, &fullscreenVao);

  systems.push_back(std::make_unique<CameraSyncSystem>(camera));
  systems.push_back(std::make_unique<RenderSystem>(this));
}

void Rasterizer::LoadScene(const std::string &fileName)
{
  Assimp::Importer importer;
  unsigned int importOptions =
      aiProcess_Triangulate | aiProcess_OptimizeMeshes |
      aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;
  const aiScene *ai_scene = importer.ReadFile(fileName, importOptions);

  if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !ai_scene->mRootNode)
  {
    throw std::runtime_error("ERROR: Assimp failed to load scene: " +
                             std::string(importer.GetErrorString()));
  }

  std::string basePath = "";
  size_t lastSlash = fileName.find_last_of("/\\");
  if (lastSlash != std::string::npos)
  {
    basePath = fileName.substr(0, lastSlash + 1);
  }

  // Load Materials
  scene.materials.reserve(ai_scene->mNumMaterials);
  for (unsigned int i = 0; i < ai_scene->mNumMaterials; i++)
  {
    aiMaterial *mat = ai_scene->mMaterials[i];
    GPUMaterial gpuMat{};

    aiColor3D color(0.f, 0.f, 0.f);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, color))
      gpuMat.ambient = glm::vec4(color.r, color.g, color.b, 0.0f);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
      gpuMat.diffuse = glm::vec4(color.r, color.g, color.b, 0.0f);

    float shininess = 32.0f;
    mat->Get(AI_MATKEY_SHININESS, shininess);

    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, color))
      gpuMat.specular = glm::vec4(color.r, color.g, color.b, shininess);

    auto loadTexture = [&](aiTextureType type, float &typeVar,
                           float &indexVar)
    {
      if (mat->GetTextureCount(type) > 0)
      {
        aiString path;
        mat->GetTexture(type, 0, &path);
        std::string texPath = basePath + path.C_Str();

        GLuint textureId = SOIL_load_OGL_texture(
            texPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
                SOIL_FLAG_COMPRESS_TO_DXT);
        if (textureId == 0)
        {
          std::cerr << "SOIL loading error for " << texPath << ": "
                    << SOIL_last_result() << std::endl;
          typeVar = 0.0f;
          indexVar = -1.0f;
        }
        else
        {
          int texIndex = scene.textureIds.size();
          scene.textureIds.push_back(textureId);
          typeVar = 1.0f;
          indexVar = float(texIndex);
        }
      }
      else
      {
        typeVar = 0.0f;
        indexVar = -1.0f;
      }
    };

    gpuMat.pbrTextureTypes = glm::vec4(0.0f);
    gpuMat.pbrTextureIndices = glm::vec4(-1.0f);
    gpuMat.pbrTextureTypes2 = glm::vec4(0.0f);
    gpuMat.pbrTextureIndices2 = glm::vec4(-1.0f);

    loadTexture(aiTextureType_DIFFUSE, gpuMat.pbrTextureTypes.x,
                gpuMat.pbrTextureIndices.x);
    if (gpuMat.pbrTextureTypes.x == 0.0f)
    {
      loadTexture(aiTextureType_BASE_COLOR, gpuMat.pbrTextureTypes.x,
                  gpuMat.pbrTextureIndices.x);
    }

    loadTexture(aiTextureType_NORMALS, gpuMat.pbrTextureTypes.y,
                gpuMat.pbrTextureIndices.y);

    loadTexture(aiTextureType_METALNESS, gpuMat.pbrTextureTypes.z,
                gpuMat.pbrTextureIndices.z);

    loadTexture(aiTextureType_DIFFUSE_ROUGHNESS, gpuMat.pbrTextureTypes.w,
                gpuMat.pbrTextureIndices.w);

    loadTexture(aiTextureType_LIGHTMAP, gpuMat.pbrTextureTypes2.x,
                gpuMat.pbrTextureIndices2.x);
    if (gpuMat.pbrTextureTypes2.x == 0.0f)
    {
      loadTexture(aiTextureType_AMBIENT, gpuMat.pbrTextureTypes2.x,
                  gpuMat.pbrTextureIndices2.x);
    }

    scene.materials.push_back(gpuMat);
  }

  // Load Meshes

  for (unsigned int m = 0; m < ai_scene->mNumMeshes; m++)
  {
    aiMesh *mesh = ai_scene->mMeshes[m];

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int rowCount = 3 + 3 + 2 + 3; // Pos (3), Norm (3), UV (2), Tangent (3)

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      // Positions
      vertices.push_back(mesh->mVertices[i].x);
      vertices.push_back(mesh->mVertices[i].y);
      vertices.push_back(mesh->mVertices[i].z);

      // Normals
      if (mesh->HasNormals())
      {
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);
      }
      else
      {
        vertices.insert(vertices.end(), {0.f, 0.f, 0.f});
      }

      // UVs
      if (mesh->HasTextureCoords(0))
      {
        vertices.push_back(mesh->mTextureCoords[0][i].x);
        vertices.push_back(mesh->mTextureCoords[0][i].y);
      }
      else
      {
        vertices.insert(vertices.end(), {0.f, 0.f});
      }

      // Tangents
      if (mesh->HasTangentsAndBitangents())
      {
        vertices.push_back(mesh->mTangents[i].x);
        vertices.push_back(mesh->mTangents[i].y);
        vertices.push_back(mesh->mTangents[i].z);
      }
      else
      {
        vertices.insert(vertices.end(), {0.f, 0.f, 0.f});
      }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    // Generate OpenGL buffers for this mesh
    Mesh newMesh;
    newMesh.indexCount = indices.size();
    newMesh.materialIndex = mesh->mMaterialIndex;

    glGenVertexArrays(1, &newMesh.vao);
    glGenBuffers(1, &newMesh.vbo);
    glGenBuffers(1, &newMesh.ebo);

    glBindVertexArray(newMesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, newMesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // Attributes
    auto numBytes = rowCount * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)0); // Pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)(3 * sizeof(float))); // Norm
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)(6 * sizeof(float))); // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)(8 * sizeof(float))); // Tangent
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    std::vector<unsigned int> adjacencyIndices = BuildAdjacencyIndices(indices);
    glGenBuffers(1, &newMesh.adjacencyEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.adjacencyEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 adjacencyIndices.size() * sizeof(unsigned int),
                 adjacencyIndices.data(), GL_STATIC_DRAW);
    newMesh.adjacencyIndexCount = static_cast<int>(adjacencyIndices.size());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.ebo);

    scene.meshes.push_back(newMesh);
  }

  auto rootEntity = registry.create();
  registry.emplace<attributes::Name>(rootEntity, fileName);

  attributes::Transform rootTransform;
  rootTransform.scale = glm::vec3(0.005f);
  rootTransform.rot = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
  rootTransform.pos = glm::vec3(0.0f); // Default to 0
  registry.emplace<attributes::Transform>(rootEntity, rootTransform);
  registry.emplace<attributes::Togglable>(rootEntity, true);

  auto &childrenAttr = registry.emplace<attributes::Children>(rootEntity);

  size_t startMeshIdx = scene.meshes.size() - ai_scene->mNumMeshes;
  for (unsigned int i = 0; i < ai_scene->mNumMeshes; ++i)
  {
    auto childEntity = registry.create();

    std::string childName = ai_scene->mMeshes[i]->mName.length > 0
                                ? ai_scene->mMeshes[i]->mName.C_Str()
                                : "Mesh " + std::to_string(i);
    registry.emplace<attributes::Name>(childEntity, childName);

    attributes::Transform childTransform;
    childTransform.scale = glm::vec3(1.0f); // Relative to parent
    childTransform.rot = glm::vec3(0.0f);
    childTransform.pos = glm::vec3(0.0f);
    registry.emplace<attributes::Transform>(childEntity, childTransform);

    registry.emplace<attributes::Togglable>(childEntity, true);
    registry.emplace<attributes::Parent>(childEntity, rootEntity);

    std::vector<int> meshIndices = {static_cast<int>(startMeshIdx + i)};
    registry.emplace<attributes::RenderMesh>(childEntity, meshIndices);

    childrenAttr.entities.push_back(childEntity);
  }
}

void Rasterizer::CreateAxes()
{
  // We mock a tiny mesh with bounds indicating the X, Y, Z coordinates
  // independently.

  auto addAxis = [&](glm::vec3 color, glm::vec3 scale, std::string name)
  {
    // Very crude line box from [-1, 1] scaled drastically
    std::vector<float> vertices = {
        -1, -1, -1, 0, 1, 0, 0, 0, 0, 0, 0, 1, -1, -1, 0, 1, 0, 0, 0, 0, 0, 0,
        1, 1, -1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, -1, 0, 1, 0, 0, 0, 0, 0, 0,
        -1, -1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, -1, 1, 0, 1, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0};
    // Map to simple triangles
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                                         7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                                         4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};

    GPUMaterial mat{};
    mat.ambient = glm::vec4(color, 1.0f);
    mat.diffuse = glm::vec4(color, 1.0f);
    mat.specular = glm::vec4(0.0f);
    mat.pbrTextureTypes = glm::vec4(0.0f);
    mat.pbrTextureIndices = glm::vec4(-1.0f);
    mat.pbrTextureTypes2 = glm::vec4(0.0f);
    mat.pbrTextureIndices2 = glm::vec4(-1.0f);

    scene.materials.push_back(mat);
    int matIndex = scene.materials.size() - 1;

    Mesh rect;
    rect.indexCount = indices.size();
    rect.materialIndex = matIndex;

    glGenVertexArrays(1, &rect.vao);
    glGenBuffers(1, &rect.vbo);
    glGenBuffers(1, &rect.ebo);
    glBindVertexArray(rect.vao);
    glBindBuffer(GL_ARRAY_BUFFER, rect.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    auto numBytes = 11 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numBytes, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, numBytes,
                          (GLvoid *)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    scene.meshes.push_back(rect);

    auto entity = registry.create();
    registry.emplace<attributes::Name>(entity, name);

    attributes::Transform transform;
    transform.scale = scale;
    transform.pos = scale; // Matches previous behavior where pos=scale
    transform.rot = glm::vec3(0.0f);
    registry.emplace<attributes::Transform>(entity, transform);

    registry.emplace<attributes::RenderMesh>(
        entity, std::vector<int>{static_cast<int>(scene.meshes.size() - 1)});

    attributes::CameraSync sync;
    // Set some default relative position for axes based on their original scale
    // or similar... The previous implementation relied on the object's
    // transform directly. For CameraSync, we just place them relative to camera
    // inverse view.
    glm::mat4 rel = glm::mat4(1.0f);
    rel = glm::translate(
        rel, glm::vec3(0.0f, 0.0f, -6.0f)); // placed 6 units in front of camera
    rel = glm::translate(rel, scale);       // match pos=scale
    rel = glm::scale(rel, scale);
    sync.relativeMatrix = rel;

    registry.emplace<attributes::CameraSync>(entity, sync);
    registry.emplace<attributes::RenderOnTop>(entity);
    registry.emplace<attributes::IsAxis>(entity);
    registry.emplace<attributes::Togglable>(
        entity, this->showAxes); // start with showAxes
  };

  // Create 3 thin boxes mapped along X, Y, Z axes
  addAxis(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.02f, 0.02f),
          "Axis: X (Red)");
  addAxis(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.02f, 1.0f, 0.02f),
          "Axis: Y (Green)");
  addAxis(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.02f, 0.02f, 1.0f),
          "Axis: Z (Blue)");
}

void Rasterizer::InitBuffers()
{
  // Meshes VBO/VAO naturally instantiated in LoadScene per your requirement.
  // If you need global buffers, they would go here.
}

void Rasterizer::InitMaterials(int bindingPoint)
{
  if (scene.materials.empty())
    return;

  glGenBuffers(1, &materialSSBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialSSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               scene.materials.size() * sizeof(GPUMaterial),
               scene.materials.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, materialSSBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Rasterizer::InitIBLTextures()
{
  LoadEXRTexture("../models/brdf_integration_map_ct_ggx.exr", brdfLUTMap);
  LoadEXRTexture("../models/lebombo_irradiance_map.exr", irradianceMap);

  LoadPrefilteredEnvMap({"../models/lebombo_prefiltered_env_map_001.exr",
                         "../models/lebombo_prefiltered_env_map_125.exr",
                         "../models/lebombo_prefiltered_env_map_250.exr",
                         "../models/lebombo_prefiltered_env_map_375.exr",
                         "../models/lebombo_prefiltered_env_map_500.exr",
                         "../models/lebombo_prefiltered_env_map_625.exr",
                         "../models/lebombo_prefiltered_env_map_750.exr",
                         "../models/lebombo_prefiltered_env_map_875.exr",
                         "../models/lebombo_prefiltered_env_map_999.exr"},
                        prefilteredMap);
}

void Rasterizer::InitShadowMap()
{
  glGenFramebuffers(1, &shadowFBO);

  glGenTextures(1, &shadowDepthMap);
  glBindTexture(GL_TEXTURE_2D, shadowDepthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  const GLfloat borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         shadowDepthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    throw std::runtime_error("ERROR: Shadow framebuffer is incomplete");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Rasterizer::LoadEXRTexture(const char *filepath, GLuint &texID,
                                bool isSrgb, bool isMipmap)
{
  float *out;
  int width;
  int height;
  const char *err = nullptr;
  int ret = LoadEXR(&out, &width, &height, filepath, &err);

  if (ret != TINYEXR_SUCCESS)
  {
    if (err)
    {
      std::cerr << "ERR (" << filepath << "): " << err << std::endl;
      free((void *)err);
    }
    return;
  }

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
               GL_FLOAT, out);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  free(out);
}

void Rasterizer::LoadPrefilteredEnvMap(
    const std::vector<std::string> &filepaths, GLuint &texID)
{
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  for (size_t i = 0; i < filepaths.size(); ++i)
  {
    float *out;
    int width;
    int height;
    const char *err = nullptr;
    int ret = LoadEXR(&out, &width, &height, filepaths[i].c_str(), &err);

    if (ret != TINYEXR_SUCCESS)
    {
      if (err)
      {
        std::cerr << "ERR (" << filepaths[i] << "): " << err << std::endl;
        free((void *)err);
      }
      continue;
    }

    glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_FLOAT, out);
    free(out);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, filepaths.size() - 1);
}

void Rasterizer::RenderDepthPass()
{
  if (!depthProgram || shadowFBO == 0 || shadowDepthMap == 0)
  {
    return;
  }

  const float t = static_cast<float>(glfwGetTime());
  const float a = t * lightAnimationSpeed;
  animatedLightDirection =
      glm::normalize(glm::vec3(cos(a), -1.0f, sin(a)));

  glm::vec3 sceneCenter(0.0f);
  glm::vec3 lightPos = sceneCenter - animatedLightDirection * 18.0f;
  glm::mat4 lightView =
      glm::lookAt(lightPos, sceneCenter, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 60.0f);
  lightSpaceMatrix = lightProj * lightView;

  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  depthProgram->activate();
  GLuint lsLoc =
      glGetUniformLocation(depthProgram->getProgramID(), "lightSpaceMatrix");
  GLuint mMatLoc =
      glGetUniformLocation(depthProgram->getProgramID(), "modelMatrix");
  glUniformMatrix4fv(lsLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

  auto view = registry.view<attributes::Transform, attributes::RenderMesh>();
  auto isVisibleWithParents = [&](entt::entity entity)
  {
    entt::entity current = entity;
    while (registry.valid(current))
    {
      if (registry.all_of<attributes::Togglable>(current) &&
          !registry.get<attributes::Togglable>(current).visible)
      {
        return false;
      }
      if (registry.all_of<attributes::Parent>(current))
      {
        current = registry.get<attributes::Parent>(current).entity;
      }
      else
      {
        break;
      }
    }
    return true;
  };

  auto computeModelMatrix = [&](entt::entity entity)
  {
    glm::mat4 modelMatrix(1.0f);
    entt::entity current = entity;
    while (registry.valid(current))
    {
      if (registry.all_of<attributes::Transform>(current))
      {
        auto &tr = registry.get<attributes::Transform>(current);
        glm::mat4 localMatrix(1.0f);
        if (tr.useMatrix)
        {
          localMatrix = tr.modelMatrix;
        }
        else
        {
          localMatrix = glm::translate(localMatrix, tr.pos);
          localMatrix =
              glm::rotate(localMatrix, tr.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
          localMatrix =
              glm::rotate(localMatrix, tr.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
          localMatrix =
              glm::rotate(localMatrix, tr.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
          localMatrix = glm::scale(localMatrix, tr.scale);
        }
        modelMatrix = localMatrix * modelMatrix;
      }
      if (registry.all_of<attributes::Parent>(current))
      {
        current = registry.get<attributes::Parent>(current).entity;
      }
      else
      {
        break;
      }
    }
    return modelMatrix;
  };

  view.each([&](entt::entity entity, attributes::Transform &, attributes::RenderMesh &renderMesh)
            {
    if (registry.all_of<attributes::RenderOnTop>(entity))
    {
      return;
    }
    if (!isVisibleWithParents(entity))
    {
      return;
    }

    glm::mat4 modelMatrix = computeModelMatrix(entity);
    glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    for (int meshIdx : renderMesh.meshIndices)
    {
      const auto &mesh = scene.meshes[meshIdx];
      glBindVertexArray(mesh.vao);
      glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    } });

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width, height);
}

void Rasterizer::RenderStencilShadowPass(const glm::mat4 &viewProjection)
{
  if (!useStencilShadows || !shadowVolumeProgram || activeStencilBits <= 0)
  {
    return;
  }

  glEnable(GL_STENCIL_TEST);
  glStencilMask(0xFF);
  glClear(GL_STENCIL_BUFFER_BIT);

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_CLAMP);

  glDisable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  if (showStencilDebug)
  {
    // Debug mode: mark any rasterized shadow volume fragment.
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOpSeparate(GL_FRONT_AND_BACK, GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glDisable(GL_DEPTH_TEST);
  }
  else
  {
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
  }

  shadowVolumeProgram->activate();
  glUniformMatrix4fv(
      glGetUniformLocation(shadowVolumeProgram->getProgramID(), "viewProjection"),
      1, GL_FALSE, &viewProjection[0][0]);
  glUniform3f(
      glGetUniformLocation(shadowVolumeProgram->getProgramID(), "lightDirection"),
      animatedLightDirection.x, animatedLightDirection.y, animatedLightDirection.z);
  glUniform1f(
      glGetUniformLocation(shadowVolumeProgram->getProgramID(), "extrusionDistance"),
      shadowVolumeExtrusion);
  glUniform1i(
      glGetUniformLocation(shadowVolumeProgram->getProgramID(),
                           "invertFacing"),
      shadowVolumeInvertFacing ? 1 : 0);

  auto view = registry.view<attributes::Transform, attributes::RenderMesh>();
  auto isVisibleWithParents = [&](entt::entity entity)
  {
    entt::entity current = entity;
    while (registry.valid(current))
    {
      if (registry.all_of<attributes::Togglable>(current) &&
          !registry.get<attributes::Togglable>(current).visible)
      {
        return false;
      }
      if (registry.all_of<attributes::Parent>(current))
      {
        current = registry.get<attributes::Parent>(current).entity;
      }
      else
      {
        break;
      }
    }
    return true;
  };

  auto computeModelMatrix = [&](entt::entity entity)
  {
    glm::mat4 modelMatrix(1.0f);
    entt::entity current = entity;
    while (registry.valid(current))
    {
      if (registry.all_of<attributes::Transform>(current))
      {
        auto &tr = registry.get<attributes::Transform>(current);
        glm::mat4 localMatrix(1.0f);
        if (tr.useMatrix)
        {
          localMatrix = tr.modelMatrix;
        }
        else
        {
          localMatrix = glm::translate(localMatrix, tr.pos);
          localMatrix =
              glm::rotate(localMatrix, tr.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
          localMatrix =
              glm::rotate(localMatrix, tr.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
          localMatrix =
              glm::rotate(localMatrix, tr.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
          localMatrix = glm::scale(localMatrix, tr.scale);
        }
        modelMatrix = localMatrix * modelMatrix;
      }
      if (registry.all_of<attributes::Parent>(current))
      {
        current = registry.get<attributes::Parent>(current).entity;
      }
      else
      {
        break;
      }
    }
    return modelMatrix;
  };

  view.each([&](entt::entity entity, attributes::Transform &, attributes::RenderMesh &renderMesh)
            {
    if (registry.all_of<attributes::RenderOnTop>(entity) ||
        !isVisibleWithParents(entity))
    {
      return;
    }

    glm::mat4 modelMatrix = computeModelMatrix(entity);
    glUniformMatrix4fv(
        glGetUniformLocation(shadowVolumeProgram->getProgramID(), "modelMatrix"),
        1, GL_FALSE, &modelMatrix[0][0]);

    for (int meshIdx : renderMesh.meshIndices)
    {
      const auto &mesh = scene.meshes[meshIdx];
      if (mesh.adjacencyEbo == 0 || mesh.adjacencyIndexCount <= 0)
      {
        continue;
      }

      glBindVertexArray(mesh.vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.adjacencyEbo);
      glDrawElements(GL_TRIANGLES_ADJACENCY, mesh.adjacencyIndexCount,
                     GL_UNSIGNED_INT, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    } });

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_CLAMP);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void Rasterizer::RenderShadowDarkenPass()
{
  if (!useStencilShadows || !shadowDarkenProgram || activeStencilBits <= 0)
  {
    return;
  }

  glEnable(GL_STENCIL_TEST);
  glStencilMask(0x00);
  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shadowDarkenProgram->activate();
  glUniform1f(
      glGetUniformLocation(shadowDarkenProgram->getProgramID(), "shadowOpacity"),
      glm::clamp(shadowDarkness, 0.0f, 1.0f));

  glBindVertexArray(fullscreenVao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void Rasterizer::RenderStencilDebugPass()
{
  if (!useStencilShadows || !showStencilDebug || !stencilDebugProgram ||
      activeStencilBits <= 0)
  {
    return;
  }

  glEnable(GL_STENCIL_TEST);
  glStencilMask(0x00);
  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  stencilDebugProgram->activate();
  glUniform1f(
      glGetUniformLocation(stencilDebugProgram->getProgramID(),
                           "debugOpacity"),
      glm::clamp(stencilDebugOpacity, 0.0f, 1.0f));

  glBindVertexArray(fullscreenVao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void Rasterizer::MainLoop()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  while (!glfwWindowShouldClose(window))
  {
    usingOffscreenStencilFallback =
        (useStencilShadows && stencilBits <= 0 && offscreenFbo != 0);
    activeStencilBits = usingOffscreenStencilFallback ? 8 : stencilBits;

    if (usingOffscreenStencilFallback)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, offscreenFbo);
    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(0, 0, width, height);

    const float t = static_cast<float>(glfwGetTime());
    const float a = t * lightAnimationSpeed;
    animatedLightDirection =
        glm::normalize(glm::vec3(cos(a), -1.0f, sin(a)));

    if (useShadowMapping)
    {
      RenderDepthPass();
    }

    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_STENCIL_TEST);

    controller->onFrame();

    float ratio = width / (float)height;
    glm::mat4 projection =
        glm::perspective(glm::radians(camera->getFov()), ratio,
                         camera->getZNear(), camera->getZFar());
    glm::mat4 view = camera->getViewMatrix();

    auto setupMainProgram = [&](bool enableShadowMap, int lightCount,
                                float ambientScale, float directScale)
    {
      program->activate();

      GLuint viewLoc =
          glGetUniformLocation(program->getProgramID(), "viewMatrix");
      GLuint projLoc =
          glGetUniformLocation(program->getProgramID(), "projectionMatrix");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

      for (size_t i = 0; i < scene.textureIds.size(); i++)
      {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, scene.textureIds[i]);
        std::string uniformName = "u_Textures[" + std::to_string(i) + "]";
        GLuint loc =
            glGetUniformLocation(program->getProgramID(), uniformName.c_str());
        glUniform1i(loc, i);
      }

      glActiveTexture(GL_TEXTURE13);
      glBindTexture(GL_TEXTURE_2D, brdfLUTMap);
      glUniform1i(glGetUniformLocation(program->getProgramID(), "brdfLUTMap"),
                  13);

      glActiveTexture(GL_TEXTURE14);
      glBindTexture(GL_TEXTURE_2D, irradianceMap);
      glUniform1i(glGetUniformLocation(program->getProgramID(), "irradianceMap"),
                  14);

      glActiveTexture(GL_TEXTURE15);
      glBindTexture(GL_TEXTURE_2D, prefilteredMap);
      glUniform1i(glGetUniformLocation(program->getProgramID(), "prefilteredMap"),
                  15);

      glActiveTexture(GL_TEXTURE16);
      glBindTexture(GL_TEXTURE_2D, shadowDepthMap);
      glUniform1i(glGetUniformLocation(program->getProgramID(), "shadowMap"),
                  16);
      glUniform1i(glGetUniformLocation(program->getProgramID(), "useShadowMap"),
                  enableShadowMap ? 1 : 0);

      glUniform1f(glGetUniformLocation(program->getProgramID(), "shadowBiasMin"),
                  shadowBiasMin);
      glUniform1f(glGetUniformLocation(program->getProgramID(), "shadowBiasMax"),
                  shadowBiasMax);
      glUniform1f(glGetUniformLocation(program->getProgramID(), "uAmbientScale"),
                  ambientScale);
      glUniform1f(glGetUniformLocation(program->getProgramID(), "uDirectScale"),
                  directScale);

      glUniformMatrix4fv(glGetUniformLocation(program->getProgramID(),
                                              "lights[0].lightMatrix"),
                         1, GL_FALSE, &lightSpaceMatrix[0][0]);

      GLuint lightCountLoc =
          glGetUniformLocation(program->getProgramID(), "lightCount");
      glUniform1i(lightCountLoc, lightCount);

      GLuint lightTypeLoc =
          glGetUniformLocation(program->getProgramID(), "lights[0].type");
      glUniform1i(lightTypeLoc, 1);

      GLuint lightColorLoc =
          glGetUniformLocation(program->getProgramID(), "lights[0].color");
      glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

      GLuint lightDirLoc =
          glGetUniformLocation(program->getProgramID(), "lights[0].direction");
      glUniform3f(lightDirLoc, animatedLightDirection.x,
                  animatedLightDirection.y, animatedLightDirection.z);
    };

    auto renderScene = [&]()
    {
      for (auto &sys : systems)
      {
        sys->update(registry);
      }
    };

    if (useStencilShadows)
    {
      // Pass 1: normal shading baseline (no shadow map sampling)
      glDisable(GL_STENCIL_TEST);
      setupMainProgram(useShadowMapping, 1, 1.0f, 1.0f);
      renderScene();

      // Pass 2: stencil shadow volumes (z-fail)
      RenderStencilShadowPass(projection * view);

      // Pass 3: darken only shadowed pixels using stencil mask
      RenderShadowDarkenPass();
      RenderStencilDebugPass();

      glDisable(GL_STENCIL_TEST);
    }
    else
    {
      setupMainProgram(useShadowMapping, 1, 1.0f, 1.0f);
      renderScene();
    }

    glBindVertexArray(0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DrawUI();

    ImGui::Render();

    if (usingOffscreenStencilFallback)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, offscreenFbo);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, width, height);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void Rasterizer::DrawUI()
{
  ImGui::Begin("Scene Graph");

  ImGui::Text("Application UI Mode: Active");
  ImGui::Text("Press TAB to toggle camera mode");
  ImGui::Separator();

  // Player Position
  glm::vec3 camPos = camera->getPosition();
  ImGui::Text("Player Pos: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);
  ImGui::SliderFloat("Camera Speed", &controller->cameraSpeed, 0.01f, 5.0f);
  ImGui::SliderFloat("Light Animation Speed", &lightAnimationSpeed, 0.0f,
                     2.0f);
  ImGui::SliderFloat("Shadow Bias Min", &shadowBiasMin, 0.0001f, 0.02f,
                     "%.5f", ImGuiSliderFlags_Logarithmic);
  ImGui::SliderFloat("Shadow Bias Max", &shadowBiasMax, 0.0005f, 0.05f,
                     "%.5f", ImGuiSliderFlags_Logarithmic);
  if (activeStencilBits > 0)
  {
    if (usingOffscreenStencilFallback)
    {
      ImGui::TextColored(ImVec4(0.45f, 0.85f, 1.0f, 1.0f),
                         "Stencil fallback active (offscreen 8-bit stencil)");
    }
    else
    {
      ImGui::Text("Stencil buffer bits: %d", stencilBits);
    }
  }
  else
  {
    ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.2f, 1.0f),
                       "Stencil buffer unavailable (GL_STENCIL_BITS = 0)");
  }
  ImGui::Checkbox("Use Shadow Mapping", &useShadowMapping);
  ImGui::Checkbox("Use Stencil Shadows", &useStencilShadows);
  ImGui::Checkbox("Stencil Invert Facing", &shadowVolumeInvertFacing);
  ImGui::SliderFloat("Stencil Shadow Darkness", &shadowDarkness, 0.05f, 1.0f);
  ImGui::Checkbox("Show Stencil Debug", &showStencilDebug);
  ImGui::SliderFloat("Stencil Debug Opacity", &stencilDebugOpacity, 0.05f,
                     1.0f);
  ImGui::SliderFloat("Stencil Extrusion", &shadowVolumeExtrusion, 20.0f,
                     400.0f);
  if (shadowBiasMin > shadowBiasMax)
  {
    shadowBiasMin = shadowBiasMax;
  }
  ImGui::Separator();

  ImGui::Text("Objects");

  auto drawEntity = [&](entt::entity entity, auto &drawEntityRef) -> void
  {
    auto &nameAttr = registry.get<attributes::Name>(entity);
    auto &transform = registry.get<attributes::Transform>(entity);

    bool hasChildren =
        registry.all_of<attributes::Children>(entity) &&
        !registry.get<attributes::Children>(entity).entities.empty();

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (!hasChildren)
    {
      flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    bool nodeOpen =
        ImGui::TreeNodeEx((void *)(intptr_t)entt::to_integral(entity), flags,
                          "%s", nameAttr.name.c_str());

    if (nodeOpen || !hasChildren) // If it's a leaf, it doesn't push, so we just
                                  // draw its controls
    {
      ImGui::PushID((int)entt::to_integral(entity));

      if (registry.all_of<attributes::Togglable>(entity))
      {
        auto &togglable = registry.get<attributes::Togglable>(entity);
        ImGui::Checkbox("Visible", &togglable.visible);
      }

      if (!transform.useMatrix)
      {
        ImGui::DragFloat3("Position", glm::value_ptr(transform.pos), 0.1f);

        glm::vec3 rotDegrees = glm::degrees(transform.rot);
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotDegrees), 1.0f))
        {
          transform.rot = glm::radians(rotDegrees);
        }

        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.001f);
      }
      else
      {
        ImGui::Text(
            "Transform is controlled by matrix externally (e.g. CameraSync).");
      }

      if (registry.all_of<attributes::RenderMesh>(entity))
      {
        auto &renderMesh = registry.get<attributes::RenderMesh>(entity);
        if (renderMesh.meshIndices.size() > 0)
          ImGui::Text("Contained Meshes: %zu", renderMesh.meshIndices.size());
      }

      if (hasChildren && nodeOpen)
      {
        auto &children = registry.get<attributes::Children>(entity);
        for (auto child : children.entities)
        {
          drawEntityRef(child, drawEntityRef);
        }
        ImGui::TreePop(); // Pop if we are a parent node and opened
      }

      ImGui::PopID();
    }
  };

  auto view = registry.view<attributes::Name, attributes::Transform>();
  view.each([&](entt::entity entity, attributes::Name &, attributes::Transform &)
            {
    // Only draw root entities (no Parents)
    if (!registry.all_of<attributes::Parent>(entity))
    {
      drawEntity(entity, drawEntity);
    } });

  ImGui::End();
}
