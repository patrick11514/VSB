#pragma once

/*
 * File: ObjectFactory.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File containing Factory for constructing Object either from
 * float data or obj file
 */

#include <memory>
#include <string>
#include <vector>

#include "../Object/DrawableObject.hpp"
#include "../Object/ObjectData.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Transformation/Transformation.hpp"

// clang-format off

/*

Factories Path

ObjectFactory ----> ObjObjectFactory ----------> ObjectDataFactory --> FinalObjectFactory
                \                            /
                 +--> PointObjectFactory -->/
*/

// clang-format on

/**
 * @brief Final factory, which then creates final Drawable Object
 */
class FinalObjectFactory {
private:
  std::shared_ptr<ObjectData> objectData;
  FinalObjectFactory(std::shared_ptr<ObjectData> objectData)
      : objectData(objectData) {};

public:
  DrawableObject *finish(ShaderProgram *shaderProgram,
                         std::shared_ptr<Transformation> transformations,
                         std::shared_ptr<Material> material,
                         RenderFunction renderFunction = nullptr);

  friend class ObjectDataFactory;
};

//

/**
 * @brief Object Data Factory create the ObjectData from model and puts it into
 * next FinalObjectFactory
 */
class ObjectDataFactory {
private:
  Model model;
  std::vector<unsigned int> indicies;

  ObjectDataFactory(Model model) : model(model) {};
  ObjectDataFactory(Model model, const std::vector<unsigned int> indicies)
      : model(model), indicies(indicies) {};

public:
  std::shared_ptr<FinalObjectFactory>
  slice(int slices = 1,
        std::function<int()> sliceAttrs = ObjectData::defaultSlice);

  friend class PointObjectFactory;
  friend class ObjObjectFactory;
};

//

/**
 * @brief Point Factory for putting float data into model
 */
class PointObjectFactory {
public:
  std::shared_ptr<ObjectDataFactory>
  putPoints(const std::vector<float> &points);
  std::shared_ptr<ObjectDataFactory> putPoints(const float *points,
                                               size_t size);
};

/**
 * @brief Obj Factory for loading data from obj file
 */
class ObjObjectFactory {
public:
  std::shared_ptr<ObjectDataFactory> loadModel(const std::string &path);
};

//

/**
 * @brief Object factory, which is starting point for Object Creating
 */
class ObjectFactory {
public:
  static std::shared_ptr<PointObjectFactory> pointObject() {
    return std::make_shared<PointObjectFactory>();
  };
  static std::shared_ptr<ObjObjectFactory> objObject() {
    return std::make_shared<ObjObjectFactory>();
  };
};
