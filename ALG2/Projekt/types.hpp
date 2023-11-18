#pragma once
#include <vector>

/**
 * @brief An enum that represents the faces of a cube.
 */
enum Face
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

/**
 * @brief An enum that represents the axies which the cubes are scanned at.
 */
enum Axis
{
    X,
    Y,
    Z,
    XInvert,
    YInvert,
    ZInvert
};

/**
 * @brief An enum that represents the status of a cube.
 */
enum Status
{
    Unchecked,
    Checked
};

/**
 * @brief A struct that represents the data of the scanning of a cube from specific axis.
 */
struct
{
    std::vector<Face> neighborsAtFaces; ///< The faces which we want to look for neighbors.
    Face checkIfFaceHasNeighbor;        ///< The face which must not have neighbor if the cube want to be counted as part of face.
} typedef CubeScanData;

/**
 * Retrieves the faces at the specified axis.
 *
 * @param axis The axis to retrieve the faces from.
 * @return A vector of Face objects representing the faces at the specified axis.
 */
std::vector<Face> getFacesAtAxis(Axis axis);
/**
 * Retrieves the CubeScanData at the specified axis.
 *
 * @param axis The axis to retrieve the CubeScanData from.
 * @return The CubeScanData at the specified axis.
 */
CubeScanData getCubeDataAtAxis(Axis axis);
/**
 * Retrieves all the axies.
 *
 * @return A vector containing all the axies.
 */
std::vector<Axis> getAllAxies();