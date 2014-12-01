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

#include "md3_importer.h"

using namespace mariachi::importers;
using namespace mariachi::exceptions;

/**
 * Constructor fo the class.
 */
Md3Importer::Md3Importer() : ModelImporter() {
}

/**
 * Destructor of the class.
 */
Md3Importer::~Md3Importer() {
}

/**
 * Generates the model information from the model file in the
 * given file path.
 *
 * @param filePath The file path to the file to be used to generate
 * the model.
 */
void Md3Importer::generateModel(const std::string &filePath) {
    // cleans the previous model information (in case there is one)
    //this->cleanModel();

    // cleans the previous frame information (in case there is one)
    //this->cleanMd3FrameList();

    // creates the file stream to be used
    std::fstream md3File(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(md3File.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // seeks to the end of the file
    md3File.seekg(0, std::fstream::end);

    // get length of file
    std::streamoff md3FileLength = md3File.tellg();

    // seeks to the beginning of the file
    md3File.seekg(0, std::fstream::beg);

    // allocates space for the md3 header
    Md3Header_t *md3Header = (Md3Header_t *) malloc(MD3_HEADER_SIZE);

    // reads the header data
    md3File.read((char *) md3Header, MD3_HEADER_SIZE);

    // calculates the md3 contents length
    std::streamoff md3ContentsLength = md3FileLength - MD3_HEADER_SIZE;

    // allocates space for the md3 contents
    char *md3Contents = (char *) malloc(md3ContentsLength);

    // reads the contents data
    md3File.read((char *) md3Contents, md3ContentsLength);

    // closes the file
    md3File.close();

    // in case the reading of the file fails
    if(md3File.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem reading the file");
    }

    // sets the frame count as the value of the number of frames
    // in the md3 header
    this->frameCount = md3Header->numberFrames;

    // sets the tag count as the value of the number of tags
    // in the md3 header
    this->tagCount = md3Header->numberTags;


    // sets the surface count as the value of the number of surfaces
    // in the md3 header
    this->surfaceCount = md3Header->numberSurfaces;







    // generates the coordinates list
    //this->generateCoordinatesList(md3Header, md3Contents);

    // generates the gl commands list
    //this->generateGlCommandsList(md2Header, md2Contents);


    // generates the frames list
    this->generateFramesList(md3Header, md3Contents);

    // generates the tags list
    this->generateTagsList(md3Header, md3Contents);

    // generates the surfaces list
    this->generateSurfacesList(md3Header, md3Contents);


    // releases the md3 header buffer
    free(md3Header);

    // releases the md3 contents buffer
    free(md3Contents);
}

/**
 * Generates the frames list for each of the frames contained in
 * the md3 model.
 *
 * @param md3Header The md3 model header to be used.
 * @param md3Contents The contents of the md3 model file.
 */
inline void Md3Importer::generateFramesList(Md3Header_t *md3Header, char *md3Contents) {
    // starts the frame contents pointer
    unsigned int frameContentsPointer = md3Header->offsetFrames - MD3_HEADER_SIZE;

    // iterates over all the frames in the model
    for(int index = 0; index < this->frameCount; index++) {
        // retrieves the frame header
        Md3FrameHeader_t *frameHeader = (Md3FrameHeader_t *) &md3Contents[frameContentsPointer];

        // increments the frame contents pointer
        frameContentsPointer += MD3_FRAME_HEADER_SIZE;

        // creates a new md3 frame
        Md3Frame *md3Frame = new Md3Frame();

        // adds the frame to the md3 frames list
        this->md3FramesList.push_back(md3Frame);

        // iterates over all the vertices
     /*   for(int _index = 0; _index < md2Header->numberVertices; _index++) {
            // retrieves the vertex contents
            Md2VertexContents_t *vertexContents = (Md2VertexContents_t *) &md2Contents[frameContentsPointer];

            // calculates the vertex coordinates with the scale and translation
            float vertexX = (vertexContents->vertex[0] * frameHeader->scale[0]) + frameHeader->translate[0];
            float vertexY = (vertexContents->vertex[1] * frameHeader->scale[1]) + frameHeader->translate[1];
            float vertexZ = (vertexContents->vertex[2] * frameHeader->scale[2]) + frameHeader->translate[2];

            // adds the vertex coordinates in the coordinates list
            md2Frame->coordinatesList.push_back(vertexX);
            md2Frame->coordinatesList.push_back(vertexY);
            md2Frame->coordinatesList.push_back(vertexZ);

            // increments the frame contents pointer
            frameContentsPointer += MD2_VERTEX_VALUE_SIZE;
        }*/
    }
}

/**
 * Generates the tags list for each of the frames contained in
 * the md3 model.
 *
 * @param md3Header The md3 model header to be used.
 * @param md3Contents The contents of the md3 model file.
 */
inline void Md3Importer::generateTagsList(Md3Header_t *md3Header, char *md3Contents) {
    // starts the tag contents pointer
    unsigned int tagContentsPointer = md3Header->offsetTags - MD3_HEADER_SIZE;

    // iterates over all the tags in the model
    for(int index = 0; index < this->tagCount; index++) {
        // retrieves the tag header
        Md3TagHeader_t *tagHeader = (Md3TagHeader_t *) &md3Contents[tagContentsPointer];


    }
}


inline void Md3Importer::generateSurfacesList(Md3Header_t *md3Header, char *md3Contents) {
    // starts the surface contents pointer
    unsigned int surfaceContentsPointer = md3Header->offsetSurfaces - MD3_HEADER_SIZE;

    // iterates over all the surfaces in the model
    for(int index = 0; index < this->surfaceCount; index++) {
        // retrieves the surface header
        Md3SurfaceHeader_t *surfaceHeader = (Md3SurfaceHeader_t *) &md3Contents[surfaceContentsPointer];


    }
}




/**
 * Constructor of the class.
 */
Md3Frame::Md3Frame() {
}

/**
 * Destructor of the class.
 */
Md3Frame::~Md3Frame() {
}
