#pragma once
#include "structs.hpp"
#include <embree3/rtcore.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define MAT_ELEM(mat, type, x, y)                                              \
  reinterpret_cast<type *>((mat).data + (mat).step * (y) +                     \
                           (mat).elemSize() * (x))

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(type_name)                                    \
  type_name(const type_name &);                                                \
  void operator=(const type_name &)

#define SAFE_DELETE(p)                                                         \
  {                                                                            \
    if (p != NULL) {                                                           \
      delete p;                                                                \
      p = NULL;                                                                \
    }                                                                          \
  }

#define SAFE_DELETE_ARRAY(p)                                                   \
  {                                                                            \
    if (p != NULL) {                                                           \
      delete[] p;                                                              \
      p = NULL;                                                                \
    }                                                                          \
  }

/*! \fn float template<typename T> void SafeDeleteVectorItems( std::vector<T> v
)
\brief Dealokuje v�echny prvky typu T vektoru v.
\param v Standardn� vektor.
*/
template <typename T> void SafeDeleteVectorItems(std::vector<T> v) {
  while (v.size() > 0) {
    T item = v.back();
    v.pop_back();
    SAFE_DELETE(item);
  }
}

namespace utils {
/*! \fn void swap( T & a, T & b )
\brief Prohod� hodnoty a, b.
\param a Prvni T hodnota.
\param b Druha T hodnota.
*/
template <typename T> void swap(T &a, T &b) {
  const T tmp = a;
  a = b;
  b = tmp;
}
} // namespace utils

/*! \fn float Random( const float range_min, const float range_max )
\brief Vr�t� pseudon�hodn� ��slo s norm�ln�m rozd�len�m v intervalu <range_min,
range_max).
\param range_min Doln� mez intervalu.
\param range_max Horn� mez intervalu.
\return Pseudon�hodn� ��slo.
*/
float Random(const float range_min = 0.0f, const float range_max = 1.0f);

/*! \fn long long GetFileSize64( const char * file_name )
\brief Vr�t� velikost souboru v bytech.
\param file_name �pln� cesta k souboru
*/
long long GetFileSize64(const char *file_name);

/*! \fn void PrintTime( double t )
\brief Vytiskne na stdout �as ve form�tu Dd:Mm:Ss.
\param t �as v sekund�ch.
*/
void PrintTime(double t, char *buffer);

/*! \fn std::string TimeToString( const double t )
\brief P�evede �as v sekund�ch do �et�zce ve form�tu Dd:Mm:Ss nebo Ss.m nebo m.u
\param t �as v sekund�ch.
*/
std::string TimeToString(const double t);

/*! \fn char * LTrim( char * s )
\brief O�e�e �et�zec o b�l� znaky zleva.
\param s ukazatel na �et�zec
\return Ukazatel na novou pozici v tomt� �et�zci
*/
char *LTrim(char *s);

/*! \fn char * RTrim( char * s )
\brief O�e�e �et�zec o b�l� znaky zprava.
\param s ukazatel na �et�zec
\return Ukazatel na novou pozici v tomt� �et�zci
*/
char *RTrim(char *s);

/*! \fn char * Trim( char * s )
\brief O�e�e �et�zec o b�l� znaky z obou stran.
\param s ukazatel na �et�zec
\return Ukazatel na novou pozici v tomt� �et�zci
*/
char *Trim(char *s);

void error_handler(void *user_ptr, const RTCError code, const char *str);

inline const int S_OK = 0;
inline const int S_ERR = -1;

inline glm::vec3 fromSimpleVec(const SimpleVec3f &vec) {
  return glm::vec3{vec.x, vec.y, vec.z};
}