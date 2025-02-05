#include "texture.h"
#include "color.h"

#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace CS248 {

inline void uint8_to_float( float dst[4], unsigned char* src ) {
  uint8_t* src_uint8 = (uint8_t *)src;
  dst[0] = src_uint8[0] / 255.f;
  dst[1] = src_uint8[1] / 255.f;
  dst[2] = src_uint8[2] / 255.f;
  dst[3] = src_uint8[3] / 255.f;
}

inline void float_to_uint8( unsigned char* dst, float src[4] ) {
  uint8_t* dst_uint8 = (uint8_t *)dst;
  dst_uint8[0] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[0])));
  dst_uint8[1] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[1])));
  dst_uint8[2] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[2])));
  dst_uint8[3] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[3])));
}

void Sampler2DImp::generate_mips(Texture& tex, int startLevel) {

  // NOTE: 
  // This starter code allocates the mip levels and generates a level 
  // map by filling each level with placeholder data in the form of a 
  // color that differs from its neighbours'. You should instead fill
  // with the correct data!

  // Extra credit: Implement this

  // check start level
  if ( startLevel >= tex.mipmap.size() ) {
    std::cerr << "Invalid start level"; 
  }

  // allocate sublevels
  int baseWidth  = tex.mipmap[startLevel].width;
  int baseHeight = tex.mipmap[startLevel].height;
  int numSubLevels = (int)(log2f( (float)max(baseWidth, baseHeight)));

  numSubLevels = min(numSubLevels, kMaxMipLevels - startLevel - 1);
  tex.mipmap.resize(startLevel + numSubLevels + 1);

  int width  = baseWidth;
  int height = baseHeight;
  for (int i = 1; i <= numSubLevels; i++) {

    MipLevel& level = tex.mipmap[startLevel + i];

    // handle odd size texture by rounding down
    width  = max( 1, width  / 2); assert(width  > 0);
    height = max( 1, height / 2); assert(height > 0);

    level.width = width;
    level.height = height;
    level.texels = vector<unsigned char>(4 * width * height);

  }

  // fill all 0 sub levels with interchanging colors
  Color colors[3] = { Color(1,0,0,1), Color(0,1,0,1), Color(0,0,1,1) };
  for(size_t i = 1; i < tex.mipmap.size(); ++i) {

    Color c = colors[i % 3];
    MipLevel& mip = tex.mipmap[i];

    for(size_t i = 0; i < 4 * mip.width * mip.height; i += 4) {
      float_to_uint8( &mip.texels[i], &c.r );
    }
  }

}

Color Sampler2DImp::sample_nearest(Texture& tex, 
                                   float u, float v, 
                                   int level) {

  // Task 4: Implement nearest neighbour interpolation
  auto mipmap = tex.mipmap[level];
  int w = min((int)mipmap.width, max(0, (int)round(u * mipmap.width))), h = min((int)mipmap.height, max(0, (int)round(v * mipmap.height)));

  int base_color_offset = 4 * (w + h * mipmap.width);
  return Color(
    (float)mipmap.texels[base_color_offset] / 255,
    (float)mipmap.texels[base_color_offset + 1] / 255,
    (float)mipmap.texels[base_color_offset + 2] / 255,
    (float)mipmap.texels[base_color_offset + 3] / 255);
}

Color Sampler2DImp::sample_bilinear(Texture& tex, 
                                    float u, float v, 
                                    int level) {
  
  // Task 4: Implement bilinear filtering

  auto mipmap = tex.mipmap[level];
  int floor_w = max(0, (int)floor(u * mipmap.width)),  floor_h = max(0, (int)floor(v * mipmap.height));

  double s = u * mipmap.width, t = v * mipmap.height;

  Color ret(0.0, 0.0, 0.0, 0.0);
  for (int i = 0; i <= 1; i++) {
    for (int j = 0; j <= 1; j++) {
      int now_w = min((int)mipmap.width, floor_w + i);
      int now_h = min((int)mipmap.height, floor_h + j);

      double proportion = (1 - abs(now_w - s)) * (1 - abs(now_h - t));
      int base_color_offset = 4 * (now_w + now_h * mipmap.width);
      ret.r += (mipmap.texels[base_color_offset] / 255.0) * proportion;
      ret.g += (mipmap.texels[base_color_offset + 1] / 255.0) * proportion;
      ret.b += (mipmap.texels[base_color_offset + 2] / 255.0) * proportion;
      ret.a += (mipmap.texels[base_color_offset + 3] / 255.0) * proportion;
    }
  }

  return ret;
}

Color Sampler2DImp::sample_trilinear(Texture& tex, 
                                     float u, float v, 
                                     float u_scale, float v_scale) {

  // Extra credit: Implement trilinear filtering

  // return magenta for invalid level
  return Color(1,0,1,1);

}

} // namespace CS248
