#pragma once

#include "Object.hpp"
#include "ObjectDataStorage.hpp"

static ObjectDataStorage dataStorage; ///< Global ObjectData storage

/**
 * @brief Create Ball from data located at /Object/Models/sphere.h
 * @param shaderProgram ShaderProgram used to draw Ball
 * @param transformations Transformation which will be applied on Object
 * @return Ball Object
 */
Object createBall(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations);
/**
 * @brief Create Tree from data located at /Object/Models/tree.h
 * @param shaderProgram ShaderProgram used to draw Tree
 * @param transformations Transformation which will be applied on Object
 * @return Tree Object
 */
Object createTree(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations);
/**
 * @brief Create Bush from data located at /Object/Models/bushes.h
 * @param shaderProgram ShaderProgram used to draw Bush
 * @param transformations Transformation which will be applied on Object
 * @return Bush Object
 */
Object createBush(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations);
/**
 * @brief Create Gift from data located at /Object/Models/gift.h
 * @param shaderProgram ShaderProgram used to draw Gift
 * @param transformations Transformation which will be applied on Object
 * @return Ball Gift
 */
Object createGift(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations);
