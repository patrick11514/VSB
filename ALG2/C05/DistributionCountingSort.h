#pragma once

#include <vector>
#include <string>

std::vector<char> loadFromFile(std::string fileName);
void putToFile(std::string fileName, std::vector<char>);
std::vector<int> distributionCountingSort(std::vector<int> input);