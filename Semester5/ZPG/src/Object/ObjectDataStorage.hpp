#pragma once

/*
 * File: ObjectDataStorage.hpp
 * Author: Patrik Mintěl (MIN0150)
 * Description: File contains ObjectDataStorage class
 */

#include <memory>
#include <unordered_map>

#include "ObjectData.hpp"

/**
 * @brief Basic storage used to store Sliced Data of objects, because we dont'
 * want to copy points for each object :)
 * @note 500 trees and 800 bushes = 24GB of RAM :)
 */
class ObjectDataStorage {
private:
  std::unordered_map<std::string, std::shared_ptr<ObjectData>>
      objectDatas; ///< storage ob ObjectDatas

public:
  /**
   * @brief Check if storage already constains ObjectData
   * @param name Name of the ObjectData
   * @return true if we containt ObjectData named by name otherwise returns
   * false
   */
  bool hasObject(const std::string &name) const;
  /**
   * @brief Get object by name
   * @param name Name of the ObjectData
   * @return Pointer to ObjectData
   * @throws std::runtime_error if ObjectData named by name was not found
   */
  std::shared_ptr<ObjectData> getObject(const std::string &name) const;
  /**
   * @brief Add new ObjectData called name
   * @param name name of ObjectData
   * @param data ObjectData to be stored
   */
  void add(const std::string &name, ObjectData data);
};
