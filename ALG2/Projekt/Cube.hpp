#pragma once
#include "types.hpp"
#include <string>

/**
 * @brief A class that represents a cube.
 */
class Cube
{
private:
    std::vector<Cube *> neighbors;     ///< The neighbors of the cube.
    Status status = Status::Unchecked; ///< The status of the cube.

    int x; ///< The x-coordinate of the cube.
    int y; ///< The y-coordinate of the cube.
    int z; ///< The z-coordinate of the cube.

public:
    /**
     * @brief Constructs a new Cube object.
     *
     * @param x The x-coordinate of the cube.
     * @param y The y-coordinate of the cube.
     * @param z The z-coordinate of the cube.
     * @return A pointer to the newly created Cube object.
     */
    Cube(int x, int y, int z);

    /**
     * @brief Adds a neighboring cube to the current cube at the specified face.
     *
     * @param cube Pointer to the neighboring cube.
     * @param face The face of the current cube where the neighboring cube is added.
     */
    void addNeighbor(Cube *cube, Face face);
    /**
     * @brief Walks through the neighbors of the cube along the specified axis.
     *
     * @param axis The axis along which to walk through the neighbors.
     * @return 1 if the cube has empty face, 0 otherwise.
     */
    int walkThroughNeighbors(Axis axis);
    /**
     * Retrieves the neighbors of the cube at the specified axis.
     *
     * @param axis The axis along which to retrieve the neighbors.
     * @return A vector containing pointers to the neighboring cubes.
     */
    std::vector<Cube *> getNeighborsAtAxis(Axis axis);
    /**
     * @brief Sets the status of the cube.
     *
     * @param status The new status of the cube.
     */
    void setStatus(Status status);

    /**
     * @brief Gets the value of the X coordinate.
     *
     * @return The value of the X coordinate.
     */
    int getX() const;
    /**
     * @brief Gets the Y coordinate of the cube.
     *
     * @return The Y coordinate of the cube.
     */
    int getY() const;
    /**
     * @brief Get the Z coordinate of the cube.
     *
     * @return int The Z coordinate of the cube.
     */
    int getZ() const;
    /**
     * @brief Get the status of the cube.
     *
     * @return The status of the cube.
     */
    Status getStatus() const;
    /**
     * @brief Converts the Cube object to a string representation.
     *
     * @return The string representation of the Cube object.
     */
    std::string toString() const;
};