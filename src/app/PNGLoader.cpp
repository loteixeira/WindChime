// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com
//
// This class is based on PNGLoader class from IrrlichtEngine (http://irrlicht.sourceforge.net/)


#include "PNGLoader.h"

extern "C"
{
	#include <png.h>
}

void PNGAPI png_read_file_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	for (png_size_t i = 0; i < length; i++)
		data[i] = getc((FILE*) png_ptr->io_ptr);
}

Texture* PNGLoader::load(string fileName, bool mipMaps)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned long sig_read = 0;
	int bit_depth, color_type, interlace_type;
	png_uint_32 width, height;
	unsigned short components;

	// Load file from disk
	FILE* file = fopen(fileName.c_str(), "rb");

	if (file == NULL)
		return NULL;

	// Create png read and info structures
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(file);
		return NULL;
	}

	info_ptr = png_create_info_struct(png_ptr);

	if (info_ptr == NULL)
	{
		fclose(NULL);
		return NULL;
	}

	// Set up the function to read bytes from file stream
	png_set_read_fn(png_ptr, file, png_read_file_data);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*) &width, (png_uint_32*) &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	// Adjust image format
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	if (bit_depth < 8)
	{
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_1_2_4_to_8(png_ptr);
		else
			png_set_packing(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	png_read_update_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*) &width, (png_uint_32*)&height, &bit_depth, &color_type, NULL, NULL, NULL);

	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
#ifdef __BIG_ENDIAN__
		png_set_swap_alpha(png_ptr);
#else
		png_set_bgr(png_ptr);
#endif
	}

	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*) &width, (png_uint_32*) &height, &bit_depth, &color_type, NULL, NULL, NULL);

	// Store the components amount
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		components = 32;
	else
		components = 24;

	// Start reading from buffer
	png_byte** row = new png_byte*[height];
	png_byte* buffer = new png_byte[width * height * (components / 8)];
	png_byte* it = buffer;

	for (png_uint_32 i = 0; i < height; i++)
	{
		row[i] = it;
		it += width * (components / 8);
	}

	png_read_image(png_ptr, row);
	png_read_end(png_ptr, NULL);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	Texture* tex = new Texture(buffer, width, height, components, mipMaps);

	delete [] row;
	delete [] buffer;

	fclose(file);

	return tex;
}
