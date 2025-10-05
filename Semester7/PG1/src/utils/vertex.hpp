#pragma once

#include "structs.hpp"

/*! \def NO_TEXTURE_COORDS
\brief Po�et texturovac�ch sou�adnic.
*/
#define NO_TEXTURE_COORDS 1

// class Surface;

/*! \struct Vertex
\brief Struktura popisuj�c� v�echny atributy vertexu.

\author Tom� Fabi�n
\version 1.0
\date 2013
*/
struct /*ALIGN*/ Vertex {
public:
  glm::vec3 position;                        /*!< Pozice vertexu. */
  glm::vec3 normal;                          /*!< Norm�la vertexu. */
  glm::vec3 color;                           /*!< RGB barva vertexu <0, 1>^3. */
  Coord2f texture_coords[NO_TEXTURE_COORDS]; /*!< Texturovac� sou�adnice. */
  glm::vec3
      tangent; /*!< Prvn� osa sou�adn�ho syst�mu tangenta-bitangenta-norm�la. */

  // char pad[8]; // dopln�n� na 64 byt�, m�lo by to m�t alespo� 4 byty, aby se
  // sem ve�el 32-bitov� ukazatel

  //! V�choz� konstruktor.
  /*!
  Inicializuje v�echny slo�ky vertexu na hodnotu nula.
  */
  Vertex() {}

  //! Obecn� konstruktor.
  /*!
  Inicializuje vertex podle zadan�ch hodnot parametr�.

  \param position pozice vertexu.
  \param normal norm�la vertexu.
  \param color barva vertexu.
  \param texture_coords nepovinn� ukazatel na pole texturovac�ch sou�adnic.
  */
  Vertex(const glm::vec3 position, const glm::vec3 normal, glm::vec3 color,
         Coord2f *texture_coords = NULL);

  // void Print();
};