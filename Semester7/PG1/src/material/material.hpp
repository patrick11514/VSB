#pragma once

#include "../texture/texture.hpp"
#include <string>

/*! \def NO_TEXTURES
\brief Maxim�ln� po�et textur p�i�azen�ch materi�lu.
*/
#define NO_TEXTURES 4

/*! \def IOR_AIR
\brief Index lomu vzduchu za norm�ln�ho tlaku.
*/
#define IOR_AIR 1.000293f

/*! \def IOR_AIR
\brief Index lomu vody.
*/
#define IOR_WATER 1.33f

/*! \def IOR_GLASS
\brief Index lomu skla.
*/
#define IOR_GLASS 1.5f

enum Shader
{
  BASIC = 1,
  GLASS = 2
};

/*! \class Material
\brief A simple material.

\author Tom� Fabi�n
\version 0.9
\date 2011-2018
*/
class Material
{
public:
  //! Implicitn� konstruktor.
  /*!
  Inicializuje v�echny slo�ky materi�lu na v�choz� matn� �ed� materi�l.
  */
  Material();

  //! Specializovan� konstruktor.
  /*!
  Inicializuje materi�l podle zadan�ch hodnot parametr�.

  \param name n�zev materi�lu.
  \param ambient barva prost�ed�.
  \param diffuse barva rozptylu.
  \param specular barva odrazu.
  \param emission  barva emise.
  \param shininess lesklost.
  \param ior index lomu.
  \param textures pole ukazatel� na textury.
  \param no_textures d�lka pole \a textures. Maxim�ln� \a NO_TEXTURES - 1.
  */
  Material(std::string &name, const glm::vec3 &ambient,
           const glm::vec3 &diffuse, const glm::vec3 &specular,
           const glm::vec3 &emission, const float reflectivity,
           const float shininess, const float ior, Texture **textures = NULL,
           const int no_textures = 0);

  //! Destruktor.
  /*!
  Uvoln� v�echny alokovan� zdroje.
  */
  ~Material();

  // void Print();

  //! Nastav� n�zev materi�lu.
  /*!
  \param name n�zev materi�lu.
  */
  void set_name(const char *name);

  //! Vr�t� n�zev materi�lu.
  /*!
  \return N�zev materi�lu.
  */
  std::string get_name() const;

  //! Nastav� texturu.
  /*!
  \param slot ��slo slotu, do kter�ho bude textura p�i�azena. Maxim�ln� \a
  NO_TEXTURES - 1.
  \param texture ukazatel na texturu.
  */
  void set_texture(const int slot, Texture *texture);

  //! Vr�t� texturu.
  /*!
  \param slot ��slo slotu textury. Maxim�ln� \a NO_TEXTURES - 1.
  \return Ukazatel na zvolenou texturu.
  */
  Texture *get_texture(const int slot) const;

public:
  glm::vec3 ambient;  /*!< RGB barva prost�ed� \f$\left<0, 1\right>^3\f$. */
  glm::vec3 diffuse;  /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
  glm::vec3 specular; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */
  glm::vec3 attenuation;

  glm::vec3 emission; /*!< RGB barva emise \f$\left<0, 1\right>^3\f$. */

  float shininess; /*!< Koeficient lesklosti (\f$\ge 0\f$). ��m je hodnota
                      v�t��, t�m se jev� povrch lesklej��. */

  float reflectivity; /*!< Koeficient odrazivosti. */
  float ior;          /*!< Index lomu. */

  Shader shader;

  static const char kDiffuseMapSlot;  /*!< ��slo slotu difuzn� textury. */
  static const char kSpecularMapSlot; /*!< ��slo slotu spekul�rn� textury. */
  static const char kNormalMapSlot;   /*!< ��slo slotu norm�lov� textury. */
  static const char kOpacityMapSlot;  /*!< ��slo slotu transparentn� textury. */

private:
  Texture *textures_[NO_TEXTURES]; /*!< Pole ukazatel� na textury. */
  /*
  slot 0 - diffuse map + alpha
  slot 1 - specular map + opaque alpha
  slot 2 - normal map
  slot 3 - transparency map
  */

  std::string name_; /*!< N�zev materi�lu. */
};