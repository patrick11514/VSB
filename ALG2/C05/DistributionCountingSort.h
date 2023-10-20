#pragma once

#include <vector>
#include <string>

/**
 * Loads data from file.
 */
std::vector<int> loadFromFile(std::string fileName);
/**
 * Puts data to file.
 */
void putToFile(std::string fileName, std::vector<int> data);

/**
 * Sorts data using distribution counting sort.
 */
std::vector<int> distributionCountingSort(std::vector<int> counts, int min, int max);
/**
 * Sorts data from file using distribution counting sort, without sotring data to memory.
 */
std::vector<int> distributionCountingSortFromFile(std::string fileName, int min, int max);