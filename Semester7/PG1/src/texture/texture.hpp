#pragma once

#include "../utils/structs.hpp"
#include "FreeImage.h"

/*! \class Texture
\brief Single texture.

\author Tom� Fabi�n
\version 0.95
\date 2012-2018
*/
class Texture {
public:
  Texture(const char *file_name);
  ~Texture();

  Color3f get_texel(const int x, const int y) const;
  Color3f get_texel(const float u, const float v) const;

  int width() const;
  int height() const;

private:
  int width_{0};        // image width (px)
  int height_{0};       // image height (px)
  int scan_width_{0};   // size of image row (bytes)
  int pixel_size_{0};   // size of each pixel (bytes)
  BYTE *data_{nullptr}; // image data in BGR format
};