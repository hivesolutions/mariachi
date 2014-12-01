// Hive Mariachi Engine
// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Mariachi Engine. If not, see <http://www.gnu.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#include "texture_importer.h"

/**
 * The bmp magic number size.
 */
#define BMP_MAGIC_SIZE 2

/**
 * The bmp header size.
 */
#define BMP_HEADER_SIZE 12

/**
 * The bmp dib v3 header size.
 */
#define BMP_DIB_V3_HEADER_SIZE 40

/**
 * The bmp maximum alpha value.
 */
#define BMP_MAXIMUM_ALPHA_VALUE 255

namespace mariachi {
    namespace importers {
        typedef struct BmpMagic_t {
            unsigned char magicNumber[2];
        } BmpMagic_t;

        typedef struct BmpHeader_t {
            unsigned int size;
            unsigned short reserved1;
            unsigned short reserved2;
            unsigned int offset;
        } BmpHeader;

        typedef struct BmpDibV3Header_t {
            unsigned int headerSize;
            unsigned int width;
            unsigned int height;
            unsigned short numberPlanes;
            unsigned short bitsPerPixel;
            unsigned int compressType;
            unsigned int bmpBytesZ;
            unsigned int horizontalResolution;
            unsigned int verticalResolution;
            unsigned int numberColors;
            unsigned int numberImportantColors;
        } BmpDibV3Header;

        typedef struct BmpRGB_t {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
        } BmpRGB;

        typedef struct BmpRGBA_t {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
            unsigned char alpha;
        } BmpRGBA;

        typedef union BmpColor_t {
            BmpRGBA_t rgba;
        } BmpColor;

        typedef struct BmpSize_t {
            unsigned int width;
            unsigned int height;
        } BmpSize;

        typedef enum BmpCompressionMethod_t {
            CMP_RGB = 0,
            CMP_RLE8,
            CMP_RLE4,
            CMP_BITFIELDS,
            CMP_JPEG,
            CMP_PNG
        } BmpCompressionMethod;

        class BmpLoader : public TextureImporter {
            private:
                BmpColor_t *bitmapData;
                BmpSize_t bitmapSize;

                inline void initBitmapData();

            public:
                BmpLoader();
                ~BmpLoader();
                void generateImage(const std::string &filePath);
                structures::Texture *getTexture();
                BmpColor_t *getBitmapData();
                BmpSize_t getBitmapSize();
        };
    }
}
