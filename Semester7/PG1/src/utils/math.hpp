#include <math.h>
template <class T>
inline T sqr(const T x) { return x * x; }

inline float deg2rad(const float x) { return x * float(M_PI) / 180.0f; }

static const float EPSILON = 0.0001f;

// indicies of refraction
static const float AIR_INDICIE = 1.0f;