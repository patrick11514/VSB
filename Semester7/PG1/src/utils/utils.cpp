#include "utils.hpp"

#include <cstring>
#include <embree3/rtcore.h>
#include <exception>
#include <functional>
#include <random>
#include <stdexcept>

using std::mt19937;
using std::uniform_real_distribution;

typedef mt19937 Engine;
typedef uniform_real_distribution<float> Distribution;

auto uniform_generator = std::bind(Distribution(0.0f, 1.0f), Engine(1));

float Random(const float range_min, const float range_max) {
  float ksi;

  // #pragma omp critical ( random )
  {
    // ksi = static_cast<float>( rand() ) / ( RAND_MAX + 1 );
    ksi = static_cast<float>(uniform_generator());

    /*static float randoms[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,
0.9f }; static int next = 0; ksi = randoms[next]; #pragma omp atomic next++; if
( next > 10 ) next = 0;*/
  }

  return ksi * (range_max - range_min) + range_min;
}

long long GetFileSize64(const char *file_name) {
  FILE *file = fopen(file_name, "rb");

  if (file != NULL) {
    fseeko64(file, 0, SEEK_END);          // p�esun na konec souboru
    long long file_size = ftello64(file); // zji�t�n� aktu�ln� pozice
    fseeko64(file, 0, SEEK_SET);          // p�esun zp�t na za��tek
    fclose(file);
    file = NULL;

    return file_size;
  }

  return 0;
}

void PrintTime(double t, char *buffer) {
  // rozklad �asu
  int days = (int)(t / (24.0 * 60.0 * 60.0));
  int hours = (int)((t - days * 24.0 * 60.0 * 60.0) / (60.0 * 60.0));
  int minutes =
      (int)((t - days * 24.0 * 60.0 * 60.0 - hours * 60.0 * 60.0) / 60.0);
  double seconds =
      t - days * 24.0 * 60.0 * 60.0 - hours * 60.0 * 60.0 - minutes * 60.0;

  // o�et�en� chybn�ch stav� jako nap�. 1m60s a p�eveden� na korektn� z�pis
  // 2m00s
  if (seconds >= 59.5) {
    seconds = 0.0;
    minutes++;
    if (minutes == 60) {
      minutes = 0;
      hours++;
      if (hours == 24) {
        hours = 0;
        days++;
      }
    }
  }

  // tisk �asu s odfiltrov�n�m nulov�ch po��te�n�ch hodnot 0d0h10m14s => 10m14s
  if (days == 0) {
    if (hours == 0) {
      if (minutes == 0) {
        if (seconds < 1.0) {
          sprintf(buffer, "%0.0f ms", seconds * 1e+3);
        } else {
          sprintf(buffer, "%0.1f s", seconds);
        }
      } else {
        sprintf(buffer, "%d m%02.0f s", minutes, seconds);
      }
    } else {
      sprintf(buffer, "%d h%02d m%02.0f s", hours, minutes, seconds);
    }
  } else {
    sprintf(buffer, "%d d%02d h%02d m%02.0f s", days, hours, minutes, seconds);
  }
}

std::string TimeToString(const double t) {
  // rozklad �asu
  int days = static_cast<int>(t / (24.0 * 60.0 * 60.0));
  int hours = static_cast<int>((t - days * 24.0 * 60.0 * 60.0) / (60.0 * 60.0));
  int minutes = static_cast<int>(
      (t - days * 24.0 * 60.0 * 60.0 - hours * 60.0 * 60.0) / 60.0);
  double seconds =
      t - days * 24.0 * 60.0 * 60.0 - hours * 60.0 * 60.0 - minutes * 60.0;

  // o�et�en� chybn�ch stav� jako nap�. 1m60s a p�eveden� na korektn� z�pis
  // 2m00s
  if (seconds >= 59.5) {
    seconds = 0.0;
    ++minutes;

    if (minutes == 60) {
      minutes = 0;
      ++hours;

      if (hours == 24) {
        hours = 0;
        ++days;
      }
    }
  }

  char buffer[32] = {0};

  // tisk �asu s odfiltrov�n�m nulov�ch po��te�n�ch hodnot 0d0h10m14s => 10m14s
  if (days == 0) {
    if (hours == 0) {
      if (minutes == 0) {
        if (seconds < 10) {
          if (seconds < 1) {
            sprintf(buffer, "%0.1fms", seconds * 1e+3);
          } else {
            sprintf(buffer, "%0.1fs", seconds);
          }
        } else {
          sprintf(buffer, "%0.0fs", seconds);
        }
      } else {
        sprintf(buffer, "%dm%02.0fs", minutes, seconds);
      }
    } else {
      sprintf(buffer, "%dh%02dm%02.0fs", hours, minutes, seconds);
    }
  } else {
    sprintf(buffer, "%dd%02dh%02dm%02.0fs", days, hours, minutes, seconds);
  }

  return std::string(buffer);
}

char *LTrim(char *s) {
  while (isspace(*s) || (*s == 0)) {
    ++s;
  };

  return s;
}

char *RTrim(char *s) {
  char *back = s + strlen(s);

  while (isspace(*--back))
    ;

  *(back + 1) = '\0';

  return s;
}

char *Trim(char *s) { return RTrim(LTrim(s)); }

void error_handler(void *user_ptr, const RTCError code, const char *str) {
  if (code != RTC_ERROR_NONE) {
    std::string descr = str ? ": " + std::string(str) : "";

    switch (code) {
    case RTC_ERROR_UNKNOWN:
      throw std::runtime_error("RTC_ERROR_UNKNOWN" + descr);
    case RTC_ERROR_INVALID_ARGUMENT:
      throw std::runtime_error("RTC_ERROR_INVALID_ARGUMENT" + descr);
      break;
    case RTC_ERROR_INVALID_OPERATION:
      throw std::runtime_error("RTC_ERROR_INVALID_OPERATION" + descr);
      break;
    case RTC_ERROR_OUT_OF_MEMORY:
      throw std::runtime_error("RTC_ERROR_OUT_OF_MEMORY" + descr);
      break;
    case RTC_ERROR_UNSUPPORTED_CPU:
      throw std::runtime_error("RTC_ERROR_UNSUPPORTED_CPU" + descr);
      break;
    case RTC_ERROR_CANCELLED:
      throw std::runtime_error("RTC_ERROR_CANCELLED" + descr);
      break;
    default:
      throw std::runtime_error("invalid error code" + descr);
      break;
    }
  }
}