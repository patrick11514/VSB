#include "ObjectFactory.hpp"
#include "../Object/IndexedObjectData.hpp"

#include <assimp/Importer.hpp>  // C++ importerinterface
#include <assimp/postprocess.h> // Post processingflags
#include <assimp/scene.h>       // aiSceneoutputdata structure
#include <stdexcept>

Assimp::Importer importer;
unsigned int importOptions =
    aiProcess_Triangulate             // Converts polygons to triangles
    | aiProcess_OptimizeMeshes        // Reduces the number of submeshes
    | aiProcess_JoinIdenticalVertices // Removes duplicate vertices
    | aiProcess_CalcTangentSpace;     // Computes tangents and bitangents

std::shared_ptr<ObjectDataFactory>
PointObjectFactory::putPoints(const std::vector<float> &points) {
  return std::shared_ptr<ObjectDataFactory>(
      new ObjectDataFactory(Model(points)));
}

std::shared_ptr<ObjectDataFactory>
PointObjectFactory::putPoints(const float *points, size_t size) {
  return std::shared_ptr<ObjectDataFactory>(
      new ObjectDataFactory(Model(points, size)));
}

std::shared_ptr<ObjectDataFactory>
ObjObjectFactory::loadModel(const std::string &path) {
  const aiScene *scene = importer.ReadFile(path, importOptions);

  if (!scene) {
    throw std::runtime_error("obj file not found");
  }

  if (scene->mNumMeshes > 1) {
    printf("Number of meshes are more than 1\n");
  }

  aiMesh *mesh = scene->mMeshes[0];

  int rowCount = (3 + 3 + 2 + 3 /* XYZ, Normal, UV, Tangent */);

  std::vector<float> points(mesh->mNumVertices * rowCount, 0);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    if (mesh->HasPositions()) {
      points[i * rowCount + 0] = mesh->mVertices[i].x;
      points[i * rowCount + 1] = mesh->mVertices[i].y;
      points[i * rowCount + 2] = mesh->mVertices[i].z;
    }

    if (mesh->HasNormals()) {
      points[i * rowCount + 3] = mesh->mNormals[i].x;
      points[i * rowCount + 4] = mesh->mNormals[i].y;
      points[i * rowCount + 5] = mesh->mNormals[i].z;
    }

    if (mesh->HasTextureCoords(0)) {
      points[i * rowCount + 6] = mesh->mTextureCoords[0][i].x;
      points[i * rowCount + 7] = mesh->mTextureCoords[0][i].y;
    }

    if (mesh->HasTangentsAndBitangents()) {
      points[i * rowCount + 8] = mesh->mTangents[i].x;
      points[i * rowCount + 9] = mesh->mTangents[i].y;
      points[i * rowCount + 10] = mesh->mTangents[i].z;
    }
  }

  std::vector<unsigned int> indicies;

  if (mesh->HasFaces()) {
    indicies = std::vector<unsigned int>(mesh->mNumFaces * 3, 0);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      indicies[i * 3] = mesh->mFaces[i].mIndices[0];
      indicies[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
      indicies[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
    }
  }

  return std::shared_ptr<ObjectDataFactory>(
      new ObjectDataFactory(Model(points), indicies));
}

std::shared_ptr<FinalObjectFactory>
ObjectDataFactory::slice(int slices, std::function<int()> sliceAttrs) {
  if (this->indicies.size() == 0)
    return std::shared_ptr<FinalObjectFactory>(new FinalObjectFactory(
        std::make_shared<ObjectData>(this->model, slices, sliceAttrs)));

  return std::shared_ptr<FinalObjectFactory>(
      new FinalObjectFactory(std::make_shared<IndexedObjectData>(
          this->model, this->indicies, slices, sliceAttrs)));
}

DrawableObject *
FinalObjectFactory::finish(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material,
                           RenderFunction renderFunction) {
  return new DrawableObject(this->objectData, shaderProgram, transformations,
                            material, renderFunction);
}
