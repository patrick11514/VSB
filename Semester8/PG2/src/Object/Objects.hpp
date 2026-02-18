#pragma once

/*
 * File: Objects.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contain functions for creating specific object, like Ball,
 * Tree etc..
 */

#include "DrawableObject.hpp"
#include "ObjectDataStorage.hpp"

static ObjectDataStorage dataStorage; ///< Global ObjectData storage

/**
 * @brief Create Ball from data located at /Object/Models/sphere.h
 * @param shaderProgram ShaderProgram used to draw Ball
 * @param transformations Transformation which will be applied on Object
 * @return Ball Object
 */
DrawableObject *createBall(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material);
/**
 * @brief Create Tree from data located at /Object/Models/tree.h
 * @param shaderProgram ShaderProgram used to draw Tree
 * @param transformations Transformation which will be applied on Object
 * @return Tree Object
 */
DrawableObject *createTree(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material);
/**
 * @brief Create Bush from data located at /Object/Models/bushes.h
 * @param shaderProgram ShaderProgram used to draw Bush
 * @param transformations Transformation which will be applied on Object
 * @return Bush Object
 */
DrawableObject *createBush(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material);
/**
 * @brief Create Gift from data located at /Object/Models/gift.h
 * @param shaderProgram ShaderProgram used to draw Gift
 * @param transformations Transformation which will be applied on Object
 * @return Gift Object
 */
DrawableObject *createGift(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material);

/**
 * @brief Create Plane from data located at /Object/Models/plane.h
 * @param shaderProgram ShaderProgram used to draw Gift
 * @param transformations Transformation which will be applied on Object
 * @return Plane Object
 */
DrawableObject *createPlane(ShaderProgram *shaderProgram,
                            std::shared_ptr<Transformation> transformations,
                            std::shared_ptr<Material> material);

/**
 * @brief Create Plane from data located at /Object/Models/plane.h
 * @param shaderProgram ShaderProgram used to draw Gift
 * @param transformations Transformation which will be applied on Object
 * @return Plane Object
 */
DrawableObject *
createPlaneNormal(ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations,
                  std::shared_ptr<Material> material);
