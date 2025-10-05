#include "texture.hpp"
#include <algorithm>

Texture::Texture(const char *file_name) {
  // image format
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  // pointer to the image, once loaded
  FIBITMAP *dib = nullptr;
  // pointer to the image data
  BYTE *bits = nullptr;

  // check the file signature and deduce its format
  fif = FreeImage_GetFileType(file_name, 0);
  // if still unknown, try to guess the file format from the file extension
  if (fif == FIF_UNKNOWN) {
    fif = FreeImage_GetFIFFromFilename(file_name);
  }
  // if known
  if (fif != FIF_UNKNOWN) {
    // check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {
      dib = FreeImage_Load(fif, file_name);
    }
    // if the image loaded
    if (dib) {
      // retrieve the image data
      bits = FreeImage_GetBits(dib);
      // FreeImage_ConvertToRawBits()
      //  get the image width and height
      width_ = int(FreeImage_GetWidth(dib));
      height_ = int(FreeImage_GetHeight(dib));

      // if each of these is ok
      if ((bits != 0) && (width_ != 0) && (height_ != 0)) {
        // texture loaded
        scan_width_ = FreeImage_GetPitch(dib);   // in bytes
        pixel_size_ = FreeImage_GetBPP(dib) / 8; // in bytes

        data_ = new BYTE[scan_width_ * height_]; // BGR(A) format
        FreeImage_ConvertToRawBits(data_, dib, scan_width_, pixel_size_ * 8,
                                   FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK,
                                   FI_RGBA_BLUE_MASK, TRUE);
      }

      FreeImage_Unload(dib);
      bits = nullptr;
    }
  }
}

Texture::~Texture() {
  if (data_) {
    // free FreeImage's copy of the data
    delete[] data_;
    data_ = nullptr;

    width_ = 0;
    height_ = 0;
  }
}

Color3f Texture::get_texel(const int x, const int y) const {
  // assert( ( x >= 0 && x < width_ ) && ( y >= 0 && y < height_ ) );

  const int offset = y * scan_width_ + x * pixel_size_;

  if (pixel_size_ > 4 * 1) // HDR, EXR
  {
    const float r = ((float *)(data_ + offset))[0];
    const float g = ((float *)(data_ + offset))[1];
    const float b = ((float *)(data_ + offset))[2];

    return Color3f{r, g, b};
  } else // PNG, JPG etc.
  {
    const float b = data_[offset] / 255.0f;
    const float g = data_[offset + 1] / 255.0f;
    const float r = data_[offset + 2] / 255.0f;

    return Color3f{r, g, b};
  }
}

Color3f Texture::get_texel(const float u, const float v) const {
  // assert( ( u >= 0.0f && u <= 1.0f ) && ( v >= 0.0f && v <= 1.0f ) );

  // nearest neighbor interpolation
  const int x = std::max(0, std::min(width_ - 1, int(u * width_)));
  const int y = std::max(0, std::min(height_ - 1, int(v * height_)));

  return get_texel(x, y);
}

int Texture::width() const { return width_; }

int Texture::height() const { return height_; }
