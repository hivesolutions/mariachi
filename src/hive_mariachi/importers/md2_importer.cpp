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

#include "md2_importer.h"

using namespace mariachi::nodes;
using namespace mariachi::importers;
using namespace mariachi::exceptions;
using namespace mariachi::structures;

/**
 * Constructor fo the class.
 */
Md2Importer::Md2Importer() : ModelImporter() {
    // invalidates the frame count
    this->frameCount = -1;
}

/**
 * Destructor of the class.
 */
Md2Importer::~Md2Importer() {
    // cleans the model
    this->cleanModel();

    // cleans the md2 frame list
    this->cleanMd2FrameList();
}

/**
 * Generates the model information from the model file in the
 * given file path.
 *
 * @param filePath The file path to the file to be used to generate
 * the model.
 */
void Md2Importer::generateModel(const std::string &filePath) {
    // cleans the previous model information (in case there is one)
    this->cleanModel();

    // cleans the previous frame information (in case there is one)
    this->cleanMd2FrameList();

    // creates the file stream to be used
    std::fstream md2File(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(md2File.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // seeks to the end of the file
    md2File.seekg(0, std::fstream::end);

    // get length of file
    std::streamoff md2FileLength = md2File.tellg();

    // seeks to the beginning of the file
    md2File.seekg(0, std::fstream::beg);

    // allocates space for the md2 header
    Md2Header_t *md2Header = (Md2Header_t *) malloc(MD2_HEADER_SIZE);

    // reads the header data
    md2File.read((char *) md2Header, MD2_HEADER_SIZE);

    // calculates the md2 contents length
    std::streamoff md2ContentsLength = md2FileLength - MD2_HEADER_SIZE;

    // allocates space for the md2 contents
    char *md2Contents = (char *) malloc(md2ContentsLength);

    // reads the contents data
    md2File.read((char *) md2Contents, md2ContentsLength);

    // closes the file
    md2File.close();

    // in case the reading of the file fails
    if(md2File.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem reading the file");
    }

    // sets the frame count as the value of the number of frames
    // in the md2 header
    this->frameCount = md2Header->numberFrames;

    // generates the coordinates list
    this->generateCoordinatesList(md2Header, md2Contents);

    // generates the gl commands list
    this->generateGlCommandsList(md2Header, md2Contents);

    // releases the md2 header buffer
    free(md2Header);

    // releases the md2 contents buffer
    free(md2Contents);
}

/**
 * Generates the coordinates list for each of the frames contained in
 * the md2 model.
 *
 * @param md2Header The md2 model header to be used.
 * @param md2Contents The contents of the md2 model file.
 */
inline void Md2Importer::generateCoordinatesList(Md2Header_t *md2Header, char *md2Contents) {
    // starts the frame contents pointer
    unsigned int frameContentsPointer = md2Header->offsetFrames - MD2_HEADER_SIZE;

    // iterates over all the frames in the model
    for(int index = 0; index < this->frameCount; index++) {
        // retrieves the frame header
        Md2FrameHeader_t *frameHeader = (Md2FrameHeader_t *) &md2Contents[frameContentsPointer];

        // increments the frame contents pointer
        frameContentsPointer += MD2_FRAME_HEADER_SIZE;

        // creates a new md2 frame
        Md2Frame *md2Frame = new Md2Frame();

        // adds the frame to the md2 frames list
        this->md2FramesList.push_back(md2Frame);

        // iterates over all the vertices
        for(int _index = 0; _index < md2Header->numberVertices; _index++) {
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
        }
    }
}

inline void Md2Importer::generateGlCommandsList(Md2Header_t *md2Header, char *md2Contents) {
    // calculates the gl contents length
    unsigned int glContentsLength = md2Header->numberGlCommands * MD2_FLOAT_SIZE;

    // starts the gl contents pointer
    unsigned int glContentsPointer = md2Header->offsetGlCommands - MD2_HEADER_SIZE;

    // starts the gl contents end pointer
    unsigned int glContentsEndPointer = glContentsPointer + glContentsLength;

    // iterates until reaching the end
    while(glContentsPointer < glContentsEndPointer) {
        // retrieves the number of vertices
        int numberVertices = *(int *) &md2Contents[glContentsPointer];

        // adds the number of vertices to the gl commands list
        this->glCommandsList.push_back((void *) numberVertices);

        // increments the gl contents pointer
        glContentsPointer += 4;

        // in case the number of vertices is negative
        if(numberVertices < 0) {
            // uses the negationof the value
            numberVertices = numberVertices * -1;
        }

        // allocates space for the vertex texture information copy buffer
        Md2VertexTextureInformation_t *vertexTextureInformationCopyBuffer = (Md2VertexTextureInformation_t *) malloc(numberVertices * sizeof(Md2VertexTextureInformation_t));

        // iterates over all the vertices
        for(int index = 0; index < numberVertices; index++) {
            // retrieves the vertex texture information
            Md2VertexTextureInformation_t *vertexTextureInformation = (Md2VertexTextureInformation_t *) &md2Contents[glContentsPointer];

            // retrieves the texture information copy
            Md2VertexTextureInformation_t *vertexTextureInformationCopy = &vertexTextureInformationCopyBuffer[index];

            // copies the vertex texture information
            memcpy(vertexTextureInformationCopy, vertexTextureInformation, sizeof(Md2VertexTextureInformation_t));

            // recalculates the vertex texture y position (inverting the value)
            vertexTextureInformationCopy->vertexTextureY = (float) 1.0 - vertexTextureInformationCopy->vertexTextureY;

            // adds the commands to the gl commands list
            this->glCommandsList.push_back((void *) &vertexTextureInformationCopy->vertexTextureX);
            this->glCommandsList.push_back((void *) &vertexTextureInformationCopy->vertexTextureY);
            this->glCommandsList.push_back((void *) &vertexTextureInformationCopy->vertexIndex);

            // increments the gl contents pointer
            glContentsPointer += 12;
        }
    }
}

/**
 * Retrieves the main md2 frame.
 *
 * @return The main md2 frame for the md2 importer.
 */
Md2Frame *Md2Importer::getMainMd2Frame() {
    // returns the first md2 frame as the main frame
    return this->md2FramesList[0];
}

/**
 * Generates the vertex list from the list of coordinates.
 */
void Md2Importer::generateVertexList() {
    // retrieves the md2 frames list iterator
    std::vector<Md2Frame *>::iterator md2FramesListIterator = this->md2FramesList.begin();

    // iterates over all the md2 frames
    while(md2FramesListIterator != this->md2FramesList.end()) {
        // retrieves the current frame
        Md2Frame *currentMd2Frame = *md2FramesListIterator;

        // generates the vertex list for the current md2 frame
        currentMd2Frame->generateVertexList();

        // increments the md2 frames list iterator
        md2FramesListIterator++;
    }
}

/**
 * Generates the mesh list for the current vertex information
 * in the importer.
 */
void Md2Importer::generateMeshList() {
    // retrieves the md2 frames list iterator
    std::vector<Md2Frame *>::iterator md2FramesListIterator = this->md2FramesList.begin();

    // iterates over all the md2 frames
    while(md2FramesListIterator != this->md2FramesList.end()) {
        // retrieves the current md2 frame
        Md2Frame *currentMd2Frame = *md2FramesListIterator;

        // generates the mesh list for the current md2 frame
        currentMd2Frame->generateMeshList(this->glCommandsList);

        // increments the md2 frames list iterator
        md2FramesListIterator++;
    }
}

/**
 * Generates the frames list from the list of meshes
 * available.
 */
void Md2Importer::generateFrameList() {
    // retrieves the md2 frames list iterator
    std::vector<Md2Frame *>::iterator md2FramesListIterator = this->md2FramesList.begin();

     // iterates over all the md2 frames
    while(md2FramesListIterator != this->md2FramesList.end()) {
        // retrieves the current md2 frame
        Md2Frame *currentMd2Frame = *md2FramesListIterator;

        // retrieves the frame structure from the current md2 frame
        Frame_t *frame = currentMd2Frame->getFrame();

        // adds the frame to the frames list
        this->framesList.push_back(frame);

        // increments the md2 frames list iterator
        md2FramesListIterator++;
    }
}

ModelNode *Md2Importer::getModelNode() {
    // retrieves the main md2 frame
    Md2Frame *mainMd2Frame = this->getMainMd2Frame();

    // creates a new model node
    ModelNode *modelNode = new ModelNode();

    // sets the model node position
    modelNode->setPosition(0.0, 0.0, 0.0);

    // sets the mesh list in the model node
    modelNode->setMeshList(&mainMd2Frame->meshList);

    // returns the model node
    return modelNode;
}

ActorNode *Md2Importer::getActorNode() {
    // creates a new actor node
    ActorNode *actorNode = new ActorNode();

    // sets the actor node position
    actorNode->setPosition(0.0, 0.0, 0.0);

    // sets the frame list in the actor node
    actorNode->setFrameList(&this->framesList);

    // returns the actor node
    return actorNode;
}

void Md2Importer::cleanModel() {
    // in case the gl commands list is empty
    // there is nothing to clean
    if(this->glCommandsList.empty())
        return;

    // retrieves the gl commands iterator
    std::vector<void *>::iterator glCommandsListIterator = this->glCommandsList.begin();

    // iterates over all gl commands
    while(glCommandsListIterator != this->glCommandsList.end()) {
        // retrieves the number of vertices
        int numberVertices = (long long) *glCommandsListIterator;

        // in case the number of vertices is less than zero
        if(numberVertices < 0) {
            numberVertices = numberVertices * -1;
        }

        // increments the gl commands list iterator
        glCommandsListIterator++;

        // in case the number of vertices is valid
        if(numberVertices > 0) {
            // retrieves the vertex texture information buffer
            Md2VertexTextureInformation_t *mMd2VertexTextureInformationBuffer = (Md2VertexTextureInformation_t *) *glCommandsListIterator;

            // releases the vertex texture information buffer
            free(mMd2VertexTextureInformationBuffer);
        }

        // increments the gl commands list iterator
        glCommandsListIterator += numberVertices * 3;
    }

    // clears the commands list
    this->glCommandsList.clear();
}

void Md2Importer::cleanMd2FrameList() {
    // in case the md2 frames list is empty
    // there is nothing to clean
    if(this->md2FramesList.empty())
        return;

    // retrieves the md2 frames list iterator
    std::vector<Md2Frame *>::iterator md2FramesListIterator = this->md2FramesList.begin();

     // iterates over all the md2 frames
    while(md2FramesListIterator != this->md2FramesList.end()) {
        // retrieves the current md2 frame
        Md2Frame *currentMd2Frame = *md2FramesListIterator;

        // deletes the current md2 frame
        delete currentMd2Frame;

        // increments the md2 frames list iterator
        md2FramesListIterator++;
    }

    // clears the frames list
    this->framesList.clear();
}

/**
 * Constructor of the class
 */
Md2Frame::Md2Frame() {
    // invalidates the mesh count
    this->meshCount = -1;
}

/**
 * Destructor of the class
 */
Md2Frame::~Md2Frame() {
    // cleans the vertex list
    this->cleanVertextList();

    // cleans the mesh list
    this->cleanMeshList();
}

/**
 * Generates the vertex list from the list of coordinates.
 *
 * @return The number of generated vertex.
 */
void Md2Frame::generateVertexList() {
    // cleans the previous vertex list information (in case there is one)
    this->cleanVertextList();

    // retrieves the coordinates list iterator
    std::vector<float>::iterator coordinatesListIterator = this->coordinatesList.begin();

    // retrieves the coordinates list size
    size_t coordinatesListSize = this->coordinatesList.size();

    // allocates memory for the vertex buffer
    float *vertexBuffer = (float *) malloc(MD2_COORDINATE_NUMBER * coordinatesListSize * MD2_FLOAT_SIZE);

    // starts the index
    unsigned int index = 0;

    // iterates over all the coordinates
    while(coordinatesListIterator != this->coordinatesList.end()) {
        // retrieves the vertex
        float *vertex = &vertexBuffer[index];

        // retrieves the vertex information
        float vertex_x = coordinatesListIterator[0];
        float vertex_y = coordinatesListIterator[1];
        float vertex_z = coordinatesListIterator[2];

        // sets the vertex information
        vertex[0] = vertex_x;
        vertex[1] = vertex_y;
        vertex[2] = vertex_z;

        // adds the vertex to the vertex list
        this->vertexList.push_back(vertex);

        // increments the coordinates list iterator
        coordinatesListIterator += 3;

        // increments the index
        index += 3;
    }
}

/**
 * Generates the mesh list for the current vertex information
 * in the importer.
 */
void Md2Frame::generateMeshList(const std::vector<void *> &glCommandsList) {
    // cleans the previous mesh list information (in case there is one)
    this->cleanMeshList();

    // in case the mesh count is invalid
    if(this->meshCount < 0) {
        // counts the meshes
        this->countMeshes(glCommandsList);
    }

    // creates the mesh buffer
    Mesh_t *meshBuffer = (Mesh_t *) malloc(sizeof(Mesh_t) * this->meshCount);

    // creates the mesh pointer
    unsigned int meshPointer = 0;

    // starts the gl command index
    unsigned int glCommandIndex = 0;

    // iterates over all the gl commands in
    // the gl commands list
    while(glCommandsList[glCommandIndex] != 0) {
        // start the mesh type
        MeshType_t meshType = TRIANGLE_STRIP;

        // retrieves the number of vertices
        int numberVertices = (long long) glCommandsList[glCommandIndex];

        // in case the number of vertices is bigger than zero
        if(numberVertices > 0) {
            // sets the triangle strip as mesh type
            meshType = TRIANGLE_STRIP;
        } else {
            // sets the triangle fan as mesh type
            meshType = TRIANGLE_FAN;
            numberVertices = numberVertices * -1;
        }

        // increments the gl command index
        glCommandIndex++;

        // retrieves the current mesh
        Mesh *mesh = &meshBuffer[meshPointer];

        // sets the type in the mesh
        mesh->type = meshType;

        // sets the number of vertices in the mesh
        mesh->numberVertices = numberVertices;

        // allocates the vertex buffer
        float *vertexBuffer = (float *) malloc(sizeof(float) * numberVertices * 5);

        // sets the inital position of the texture vertex buffer (part of the vertex buffer)
        float *textureVertexBuffer = vertexBuffer + numberVertices * 3;

        // sets the vertex list in the mesh
        mesh->vertexList = vertexBuffer;

        // sets the texture vertex list in the mesh
        mesh->textureVertexList = textureVertexBuffer;

        // iterates over all the vertices
        for(int index = 0; index < numberVertices; index++) {
            // retrieves the vertex index
            int vertexIndex = *(int *) glCommandsList[glCommandIndex + 2];

            // retrieves the vertex
            float *vertex = this->vertexList[vertexIndex];

            // copies the vertex information to the vertex buffer
            memcpy(vertexBuffer, vertex, sizeof(float) * 3);

            // copes the texture vertex information to the vertex texture buffer
            memcpy(textureVertexBuffer, glCommandsList[glCommandIndex], sizeof(float) * 2);

            // increments the gl command index
            glCommandIndex += 3;

            // increments the vertex buffer position
            vertexBuffer += 3;

            // increments the texture vertex buffer position
            textureVertexBuffer += 2;
        }

        // adds the mesh to the mesh list
        this->meshList.push_back(mesh);

        // increments the mesh pointer
        meshPointer++;
    }
}

void Md2Frame::countMeshes(const std::vector<void *> &glCommandsList) {
    // resets the mesh count
    this->meshCount = 0;

    // starts the gl command index
    unsigned int glCommandIndex = 0;

    // iterates over all the gl commands in
    // the gl commands list
    while(glCommandsList[glCommandIndex] != 0) {
        // retrieves the number of vertices
        int numberVertices = (long long) glCommandsList[glCommandIndex];

        // in case the number of vertices is smaller than zero
        if(numberVertices < 0) {
            numberVertices = numberVertices * -1;
        }

        // increments the gl command index
        glCommandIndex += numberVertices * 3 + 1;

        // increments the mesh count
        this->meshCount++;
    }
}

/*
void Md2Importer::compileDisplayList() {
    // starts the gl command index
    unsigned int glCommandIndex = 0;

    // start the type
    unsigned char type = 0;

    // iterates over all the gl commands in
    // the gl commands list
    while(this->glCommandsList[glCommandIndex] != 0) {
        // retrieves the number of vertices
        int numberVertices = (int) this->glCommandsList[glCommandIndex];

        // in case the number of vertices is bigger than zero
        if(numberVertices > 0) {
            type = 0;
        }
        else {
            numberVertices = numberVertices * -1;
            type = 1;
        }

        // increments the gl command index
        glCommandIndex++;

        // in case the type is zero
        if(type == 0)
            glBegin(GL_TRIANGLE_STRIP);
        else
            glBegin(GL_TRIANGLE_FAN);

        // iterates over all the vertices
        for(int index = 0; index < numberVertices; index++) {
            // retrieves the vertex texture information
            float vertexTextureX = *(float *) this->glCommandsList[glCommandIndex + 0];
            float vertexTextureY = *(float *) this->glCommandsList[glCommandIndex + 1];
            int vertexIndex = *(int *) this->glCommandsList[glCommandIndex + 2];

            // retrieves the vertex
            float *vertex = this->vertexList[vertexIndex];

            // retrieves the vertex coordinates
            float vertexX = vertex[0];
            float vertexY = vertex[1];
            float vertexZ = vertex[2];

            glTexCoord2f(vertexTextureX, GLfloat(1.0) - vertexTextureY);
            glVertex3f(vertexX, vertexY, vertexZ);

            // increments the gl command index
            glCommandIndex += 3;
        }

        glEnd();
    }
}*/

Frame_t *Md2Frame::getFrame() {
    // updates the frame values
    this->frame.type = FRAME_TYPE_NORMAL;
    this->frame.name = "";
    this->frame.meshList = &this->meshList;

    // returns the frame
    return &this->frame;
}

void Md2Frame::cleanVertextList() {
    // in case the vertex list is empty
    // there is nothing to clean
    if(this->vertexList.empty())
        return;

    // retrieves the first vertex
    float *firstVertex = this->vertexList[0];

    // retrieves the vertex buffer reference
    float *vertexBuffer = (float *) &firstVertex[0];

    // releases the vertex buffer
    free(vertexBuffer);

    // clears the vertex list
    this->vertexList.clear();
}

void Md2Frame::cleanMeshList() {
    // in case the mesh list is empty
    // there is nothing to clean
    if(this->meshList.empty())
        return;

    // retrieves the mesh list iterator
    std::vector<Mesh_t *>::iterator meshListIterator = this->meshList.begin();

    // iterates over all the meshes in the mesh list
    while(meshListIterator != this->meshList.end()) {
        // retrieves the mesh
        Mesh_t *mesh = *meshListIterator;

        // retrieves the vertex buffer
        float *vertexBuffer = mesh->vertexList;

        // releases the vertex buffer
        free(vertexBuffer);

        // increments the mesh list iterator
        meshListIterator++;
    }

    // retrieves the mesh buffer
    Mesh_t *meshBuffer = this->meshList[0];

    // releases the mesh buffer
    free(meshBuffer);
}
