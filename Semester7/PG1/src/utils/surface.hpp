#pragma once

#include "../material/material.hpp"
#include "triangle.hpp"
#include <string>
#include <vector>

/*! \class Surface
\brief A class representing a triangular mesh.

\author Tom� Fabi�n
\version 0.9
\date 2012-2018
*/
class Surface {
public:
  //! V�choz� konstruktor.
  /*!
  Inicializuje v�echny slo�ky s�t� na hodnotu nula.
  */
  Surface();

  //! Obecn� konstruktor.
  /*!
  Inicializuje vertex podle zadan�ch hodnot parametr�.

  \param name n�zev plochy.
  \param n po�et troj�heln�k� tvo��c�ch s�.
  */
  Surface(const std::string &name, const size_t n);

  //! Destruktor.
  /*!
  Uvoln� v�echny alokovan� zdroje.
  */
  ~Surface();

  //! Vr�t� po�adovan� troj�heln�k.
  /*!
  \param i index troj�heln�ka.
  \return Troj�heln�k.
  */
  Triangle &get_triangle(const size_t i);

  //! Vr�t� pole v�ech troj�heln�k�.
  /*!
  \return Pole v�ech troj�heln�k�.
  */
  Triangle *get_triangles();

  //! Vr�t� n�zev plochy.
  /*!
  \return N�zev plochy.
  */
  std::string get_name();

  //! Vr�t� po�et v�ech troj�heln�k� v s�ti.
  /*!
  \return Po�et v�ech troj�heln�k� v s�ti.
  */
  size_t no_triangles();

  //! Vr�t� po�et v�ech vrchol� v s�ti.
  /*!
  \return Po�et v�ech vrchol� v s�ti.
  */
  size_t no_vertices();

  //! Nastav� materi�l plochy.
  /*!
  \param material ukazatel na materi�l.
  */
  void set_material(Material *material);

  //! Vr�t� ukazatel na materi�l plochy.
  /*!
  \return Ukazatel na materi�l plochy.
  */
  Material *get_material() const;

protected:
private:
  size_t n_{0};                  /*!< Po�et troj�heln�k� v s�ti. */
  Triangle *triangles_{nullptr}; /*!< Troj�heln�kov� s�. */

  std::string name_{"unknown"}; /*!< N�zev plochy. */

  // Matrix4x4 transformation_; /*!< Transforma�n� matice pro p�echod z
  // modelov�ho do sv�tov�ho sou�adn�ho syst�mu. */
  Material *material_{nullptr}; /*!< Materi�l plochy. */
};

/*! \fn Surface * BuildSurface( const std::string & name, std::vector<Vertex> &
face_vertices )
\brief Sestaven� plochy z pole trojic vrchol�.
\param name n�zev plochy.
\param face_vertices pole trojic vrchol�.
*/
Surface *BuildSurface(const std::string &name,
                      std::vector<Vertex> &face_vertices);