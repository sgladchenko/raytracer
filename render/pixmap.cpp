#include "png.h"

#include "render.hpp"
#include <stdlib.h>
#include <stdio.h>

void rt::Pixmap::saveFile(std::string fname, rt::Pixmap::format fmt) const
{
    switch (fmt)
    {
        case (rt::Pixmap::format::png):
            savepng(fname);
            break;
    }
}

void rt::Pixmap::savepng(std::string fname) const
{
    FILE* fp = fopen(fname.c_str(), "wb");
    if (!fp) { return; }

    // Needs to add some error handling; better to do it with some exceptions,
    // and probably create some desctructors.

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);

    if (setjmp(png_jmpbuf(png))) { return; }

    png_init_io(png, fp);
    png_set_IHDR(
        png,
        info,
        Ny, Nx,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * Nx);
    for (int i = 0; i < Nx; ++i)
    {
        row_pointers[i] = (png_bytep) malloc(sizeof(png_byte) * Ny * 4);
        for (int j = 0; j < Ny; ++j)
        {
            row_pointers[i][4*j + 0] = pixel(i,j).red;
            row_pointers[i][4*j + 1] = pixel(i,j).green;
            row_pointers[i][4*j + 2] = pixel(i,j).blue;
            row_pointers[i][4*j + 3] = 0xff; // A channel is always 255, it isn't used
        }
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    for (int i = 0; i < Nx; ++i) { free(row_pointers[i]); }
    free(row_pointers);

    fclose(fp);
    png_destroy_write_struct(&png, &info);
}
