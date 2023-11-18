#pragma once
#include <string>
#include <fstream>
#include "Cube.hpp"

/**
 * @brief A class that represents a height map.
 */
class HeightMap
{
private:
    std::vector<Cube *> cubes; ///< The cubes of the height map.
    int width;                 ///< The width of the height map.
    int length;                ///< The length of the height map.
    int height;                ///< The height of the height map.

    /**
     * @brief Sets the cube at the specified coordinates.
     *
     * @param x The x-coordinate of the cube.
     * @param y The y-coordinate of the cube.
     * @param z The z-coordinate of the cube.
     * @param cube A pointer to the Cube object to be set.
     */
    void setCube(int x, int y, int z, Cube *cube);
    /**
     * @brief Calculates the maximum height from the given file.
     *
     * This function reads the height values from the specified file and returns the maximum height found.
     *
     * @param file The input file stream to read the height values from.
     * @return The maximum height value found in the file.
     */
    int getMaxHeight(std::ifstream &file);
    /**
     * @brief Creates cubes based on the data from the input file.
     *
     * This function reads data from the input file and creates cubes accordingly.
     *
     * @param file The input file stream to read data from.
     * @return void
     */
    void createCubes(std::ifstream &file);
    /**
     * @brief Adds neighbors to corresponding cubes.
     *
     * This function goes through all the cubes and adds neighbors to them.
     *
     * @return void
     */
    void addNeighbors();

    /**
     * @brief Retrieves the Cube object at the specified coordinates.
     *
     * @param x The x-coordinate of the Cube.
     * @param y The y-coordinate of the Cube.
     * @param z The z-coordinate of the Cube.
     * @return A pointer to the Cube object at the specified coordinates, or nullptr if no Cube exists at those coordinates.
     */
    Cube *getCube(int x, int y, int z) const;
    /**
     * Returns the number of faces at the specified axis.
     *
     * @param axis The axis to get the faces from.
     * @return The number of faces at the specified axis.
     */
    int getFacesAtAxis(Axis axis) const;
    /**
     * @brief Resets the status of all the cubes to Status::Unchecked.
     *
     * @return void
     */
    void resetStatus() const;
    /**
     * @brief Returns the index of the cube at the specified coordinates.
     *
     * @param x The x-coordinate of the cube.
     * @param y The y-coordinate of the cube.
     * @param z The z-coordinate of the cube.
     * @return The index of the cube at the specified coordinates.
     */
    int getIndex(int x, int y, int z) const;

public:
    /**
     * @brief Constructs a new HeightMap object.
     *
     * @param fileName The name of the file to read the height map data from.
     */
    HeightMap(std::string fileName);
    /**
     * @brief Destroys the HeightMap object.
     */
    ~HeightMap();

    /**
     * @brief Returns the number of faces of object
     * This function returns number of faces of object specified by HeighMap
     * @return The number of faces of object
     */
    int getFaces() const;
};