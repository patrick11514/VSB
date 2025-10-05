#pragma once

#include "vertex.hpp"

class Surface; // dop�edn� deklarace t��dy

/*! \class Triangle
\brief A class representing single triangle in 3D.

\author Tom� Fabi�n
\version 1.1
\date 2013-2018
*/
class Triangle {
public:
  //! V�choz� konstruktor.
  /*!
  Inicializuje v�echny slo�ky troj�heln�ku na hodnotu nula.
  */
  Triangle() {}

  //! Obecn� konstruktor.
  /*!
  Inicializuje troj�heln�k podle zadan�ch hodnot parametr�.

  \param v0 prvn� vrchol troj�heln�ka.
  \param v1 druh� vrchol troj�heln�ka.
  \param v2 t�et� vrchol troj�heln�ka.
  \param surface ukazatel na plochu, j� je troj�heln�k �lenem.
  */
  Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2,
           Surface *surface = NULL);

  //! I-t� vrchol troj�heln�ka.
  /*!
  \param i index vrcholu troj�heln�ka.

  \return I-t� vrchol troj�heln�ka.
  */
  Vertex vertex(const int i);

  bool is_degenerate() const;

private:
  Vertex vertices_[3]; /*!< Vrcholy troj�heln�ka. Nic jin�ho tu nesm� b�t, jinak
                          padne VBO v OpenGL! */
};