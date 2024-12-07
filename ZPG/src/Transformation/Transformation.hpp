#pragma once

/*
 * File: Transformation.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains Transformation Class
 */

#include <glm/matrix.hpp>
#include <memory>
#include <vector>

/**
 * @brief Basic transformation using Composite pattern
 */
class Transformation
    : public std::enable_shared_from_this<
          Transformation> { // Enable creating shared ptr from this, because we
                            // want to chain Transformations and Object needs
                            // Shader Pointer of Transformation, so we could
                            // chain in Object's constructor
protected:
  std::vector<std::shared_ptr<Transformation>>
      transformations; ///< Stored Transfrormations

public:
  virtual ~Transformation() = default; ///< Virtual destructor

  /**
   * @brief Add transformation
   * @param tran Transformation to be added
   * @return Pointer to this, so we can chain transformations
   */
  std::shared_ptr<Transformation> addTransformation(Transformation *tran);

  virtual glm::mat4
  getMatrix(); ///< Calculate Transformation Matrix from all given
               ///< transformations TODO: implement for dynamic
               ///< transformations, so transformation, can say it will be
               ///< dynamic, then it will get the Elapsed animation time
               ///< and it can perform some calculations before returning
               ///< final matrice in getMatrix()
};
