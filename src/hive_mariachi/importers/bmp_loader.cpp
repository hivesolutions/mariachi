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

#include "stdafx.h"

#include "../exceptions/exceptions.h"

#include "bmp_loader.h"

using namespace mariachi::importers;
using namespace mariachi::exceptions;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
BmpLoader::BmpLoader() : TextureImporter() {
    this->bitmapData = NULL;
}

/**
 * Destructor of the class.
 */
BmpLoader::~BmpLoader() {
    if(this->bitmapData) {
        free(this->bitmapData);
    }
}

inline void BmpLoader::initBitmapData() {
    this->bitmapData = NULL;
}

void BmpLoader::generateImage(const std::string &filePath) {
    // creates the file stream to be used
    std::fstream bmpFile(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(bmpFile.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // seeks to the end of the file
    bmpFile.seekg(0, std::fstream::end);

    // get length of file
    std::streamoff bmpFileLength = bmpFile.tellg();

    // seeks to the beginning of the file
    bmpFile.seekg(0, std::fstream::beg);

    // allocates space for the bmp magic
    BmpHeader_t *bmpMagic = (BmpHeader_t *) malloc(BMP_MAGIC_SIZE);

    // reads the magic data
    bmpFile.read((char *) bmpMagic, BMP_MAGIC_SIZE);

    // allocates space for the bmp header
    BmpHeader_t *bmpHeader = (BmpHeader_t *) malloc(BMP_HEADER_SIZE);

    // reads the header data
    bmpFile.read((char *) bmpHeader, BMP_HEADER_SIZE);

    // allocates space for the bmp dib v3 header
    BmpDibV3Header_t *bmpDibHeader = (BmpDibV3Header_t *) malloc(BMP_DIB_V3_HEADER_SIZE);

    // reads the bmp dib v3 header data
    bmpFile.read((char *) bmpDibHeader, BMP_DIB_V3_HEADER_SIZE);

    // sets the bitmap size information
    this->bitmapSize.height = bmpDibHeader->height;
    this->bitmapSize.width = bmpDibHeader->width;

    // calculates the bitmap data length
    unsigned int bitmapDataLength = bmpHeader->size - BMP_MAGIC_SIZE - BMP_HEADER_SIZE - BMP_DIB_V3_HEADER_SIZE;

    // allocates space for the bitmap raw data
    char *bitmapRawData = (char *) malloc(bitmapDataLength);

    // reads the bitmap raw data
    bmpFile.read((char *) bitmapRawData, bitmapDataLength);

    // calculates the pixel count
    int pixelCount = bmpDibHeader->height * bmpDibHeader->width;

    // calculates the number of bytes per pixel
    unsigned char bytesPerPixel = bmpDibHeader->bitsPerPixel / 8;

    // calculates the real bitmap data length
    unsigned int realBitmapDataLength = pixelCount * bytesPerPixel;

    // allocates space for the bitmap data
    this->bitmapData = (BmpColor_t *) malloc(pixelCount * sizeof(BmpColor_t));

    // starts the current byte pointer
    unsigned int currentBytePointer = 0;

    // iterates over all the (real) bytes in the bitmap
    for(int index = 0; currentBytePointer < realBitmapDataLength; index++) {
        // sets the bitmap colors
        bitmapData[index].rgba.red = bitmapRawData[currentBytePointer + 2];
        bitmapData[index].rgba.green = bitmapRawData[currentBytePointer + 1];
        bitmapData[index].rgba.blue = bitmapRawData[currentBytePointer + 0];

        // in case the bitmap contains alpha
        if(bytesPerPixel == 4) {
            // sets the alpha color
            bitmapData[index].rgba.alpha = bitmapRawData[currentBytePointer + 3];
        }
        else {
            // sets the alpha color to the maximum
            bitmapData[index].rgba.alpha = BMP_MAXIMUM_ALPHA_VALUE;
        }

        // increments the current byte pointer
        currentBytePointer += bytesPerPixel;
    }

    // closes the file
    bmpFile.close();

    // releases the bmp magic
    free(bmpMagic);

    // release the bmp header
    free(bmpHeader);

    // releases the bmp dib header
    free(bmpDibHeader);

    // releases the bitmap raw data
    free(bitmapRawData);
}

Texture *BmpLoader::getTexture() {
    Texture *texture = new Texture();

    IntSize2d_t size;
    size.height = this->bitmapSize.height;
    size.width = this->bitmapSize.width;

    texture->setImageBuffer((ImageColor_t *) this->bitmapData);
    texture->setSize(size);

    // returns the texture
    return texture;
}

BmpColor_t *BmpLoader::getBitmapData() {
    return this->bitmapData;
}

BmpSize_t BmpLoader::getBitmapSize() {
    return this->bitmapSize;
}
