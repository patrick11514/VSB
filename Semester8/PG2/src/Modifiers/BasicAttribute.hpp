#pragma once

/*
 * File: BasiAttribute.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains BasicAttribute class
 */

/**
 * @brief Basic Attribute class is class from which inherits BasicObject, this
 * will add ability to check every other Object's Attributes (Drawable etc..)
 * using dynamic_cast
 */
class BasicAttribute {
public:
  virtual ~BasicAttribute() = default;
};
