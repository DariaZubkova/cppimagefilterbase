#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <array>
#include "stb_image_write.h"
#include "png_toolkit.h"

png_toolkit::png_toolkit()
{
}

png_toolkit::~png_toolkit()
{
    stbi_image_free(imgData.pixels);
}

bool png_toolkit::load( const std::string &pictureName )
{
    imgData.pixels = stbi_load(pictureName.c_str(), &imgData.w, &imgData.h, &imgData.compPerPixel, 0);
    return imgData.pixels != nullptr;
}

void png_toolkit::edit_image()
{
	int sizeString = imgData.compPerPixel * imgData.w;
	for (int i = imgData.h / 2; i < imgData.h; i++) {
		for (int j = 0; j < sizeString; j+= imgData.compPerPixel) {
			for (int k = 0; k < imgData.compPerPixel; k++) {
				if (k == 0)
					imgData.pixels[k + i * sizeString  + j] = (unsigned char)255;
				if (k == 1 || k == 2)
					imgData.pixels[k + i * sizeString + j] = (unsigned char)0;
			}
		}
	}
}

bool png_toolkit::save( const std::string &pictureName )
{
    return stbi_write_png(pictureName.c_str(),
                   imgData.w, imgData.h,
                   imgData.compPerPixel,
                          imgData.pixels, 0) != 0;
}


image_data png_toolkit::getPixelData( void ) const
{
    return imgData;
}
