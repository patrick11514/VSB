#include "Rasterizer.hpp"
#include "Camera.hpp"
#include "Controller.hpp"
#include "Shader/ShaderProgram.hpp"
#include "system/CameraSyncSystem.hpp"
#include "system/RenderSystem.hpp"
#include "attributes.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <soil2/SOIL2.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>
#include <memory>

Rasterizer::Rasterizer(int width, int height, const char *title)
    : width(width), height(height), title(title), window(nullptr),
      camera(nullptr), controller(nullptr), program(nullptr), materialSSBO(0) {}

Rasterizer::~Rasterizer()
{
  delete camera;
  // Window deletion handled by glfwTerminate usually
  if (window)
  {
    glfwDestroyWindow(window);
  }
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
      auto view = rast->registry.view<attributes::IsAxis, attributes::Togglable>();
      for (auto entity : view) {
        auto &tog = view.get<attributes::Togglable>(entity);
        tog.visible = rast->showAxes;
      }
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
                              "../shaders/fragment/BaseBlinn.frag", controller);

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

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
      aiString path;
      mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
      std::string texPath = basePath + path.C_Str();

      GLuint textureId = SOIL_load_OGL_texture(
          texPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
          SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
              SOIL_FLAG_COMPRESS_TO_DXT);
      if (textureId == 0)
      {
        std::cerr << "SOIL loading error for " << texPath << ": "
                  << SOIL_last_result() << std::endl;
        gpuMat.textureInfo =
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // Fallback to color
      }
      else
      {
        int texIndex = scene.textureIds.size();
        scene.textureIds.push_back(textureId);
        gpuMat.textureInfo = glm::vec4(1.0f, float(texIndex), 0.0f,
                                       0.0f); // Type 1 (Texture), Index
      }
    }
    else
    {
      gpuMat.textureInfo = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // Type 0 (Color)
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
      
      std::string childName = ai_scene->mMeshes[i]->mName.length > 0 ? 
                              ai_scene->mMeshes[i]->mName.C_Str() : 
                              "Mesh " + std::to_string(i);
      registry.emplace<attributes::Name>(childEntity, childName);
      
      attributes::Transform childTransform;
      childTransform.scale = glm::vec3(1.0f); // Relative to parent
      childTransform.rot = glm::vec3(0.0f);
      childTransform.pos = glm::vec3(0.0f);
      registry.emplace<attributes::Transform>(childEntity, childTransform);
      
      registry.emplace<attributes::Togglable>(childEntity, true);
      registry.emplace<attributes::Parent>(childEntity, rootEntity);
      
      std::vector<int> meshIndices = { static_cast<int>(startMeshIdx + i) };
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
    mat.textureInfo = glm::vec4(0.0f);

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
    
    registry.emplace<attributes::RenderMesh>(entity, std::vector<int>{static_cast<int>(scene.meshes.size() - 1)});
    
    attributes::CameraSync sync;
    // Set some default relative position for axes based on their original scale or similar...
    // The previous implementation relied on the object's transform directly.
    // For CameraSync, we just place them relative to camera inverse view.
    glm::mat4 rel = glm::mat4(1.0f);
    rel = glm::translate(rel, glm::vec3(0.0f, 0.0f, -6.0f)); // placed 6 units in front of camera
    rel = glm::translate(rel, scale); // match pos=scale
    rel = glm::scale(rel, scale);
    sync.relativeMatrix = rel;
    
    registry.emplace<attributes::CameraSync>(entity, sync);
    registry.emplace<attributes::RenderOnTop>(entity);
    registry.emplace<attributes::IsAxis>(entity);
    registry.emplace<attributes::Togglable>(entity, this->showAxes); // start with showAxes
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

void Rasterizer::MainLoop()
{
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    controller->onFrame();

    float ratio = width / (float)height;
    glm::mat4 projection =
        glm::perspective(glm::radians(camera->getFov()), ratio,
                         camera->getZNear(), camera->getZFar());
    glm::mat4 view = camera->getViewMatrix();

    program->activate();

    // Pass view / projection manually
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

    GLuint mMatLoc =
        glGetUniformLocation(program->getProgramID(), "modelMatrix");

    GLuint lightCountLoc =
        glGetUniformLocation(program->getProgramID(), "lightCount");
    glUniform1i(lightCountLoc, 1);

    GLuint lightTypeLoc =
        glGetUniformLocation(program->getProgramID(), "lights[0].type");
    glUniform1i(lightTypeLoc, 1); // DIRECTIONAL

    GLuint lightColorLoc =
        glGetUniformLocation(program->getProgramID(), "lights[0].color");
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    GLuint lightDirLoc =
        glGetUniformLocation(program->getProgramID(), "lights[0].direction");
    glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);

    for (auto &sys : systems)
    {
        sys->update(registry);
    }

    glBindVertexArray(0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DrawUI();

    ImGui::Render();
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
  ImGui::Separator();

  ImGui::Text("Objects");
  
  auto drawEntity = [&](entt::entity entity, auto& drawEntityRef) -> void {
      auto &nameAttr = registry.get<attributes::Name>(entity);
      auto &transform = registry.get<attributes::Transform>(entity);
      
      bool hasChildren = registry.all_of<attributes::Children>(entity) && !registry.get<attributes::Children>(entity).entities.empty();
      
      ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
      if (!hasChildren)
      {
          flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
      }
      
      bool nodeOpen = ImGui::TreeNodeEx((void *)(intptr_t)entt::to_integral(entity), flags, "%s", nameAttr.name.c_str());
      
      if (nodeOpen || !hasChildren) // If it's a leaf, it doesn't push, so we just draw its controls
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
              ImGui::Text("Transform is controlled by matrix externally (e.g. CameraSync).");
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
  for (auto entity : view)
  {
      // Only draw root entities (no Parents)
      if (!registry.all_of<attributes::Parent>(entity))
      {
          drawEntity(entity, drawEntity);
      }
  }

  ImGui::End();
}
