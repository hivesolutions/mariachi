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

#include "../structures/mesh.h"
#include "model_importer.h"

/**
 * The md2 header size.
 */
#define MD2_HEADER_SIZE 68

/**
 * The md2 frame header size.
 */
#define MD2_FRAME_HEADER_SIZE 40

/**
 * The md2 float size size.
 */
#define MD2_FLOAT_SIZE 4

/**
 * The md2 vertex value size.
 */
#define MD2_VERTEX_VALUE_SIZE 4

/**
 * The md2 coordinate number.
 */
#define MD2_COORDINATE_NUMBER 3

namespace mariachi {
    namespace importers {
        /**
         * The md2 header structure, containing information about
         * md2 metadata.
         *
         * @param magicNumber The md2 magic number (IDP2).
         * @param md2Version The version of the md2 implementation.
         * @param textureWidth The width of the texture.
         * @param textureHeight The height of the texture.
         * @param frameSize The size of a frame (in bytes).
         * @param numberTextures The number of textures.
         * @param numberVertices The number of vertices.
         * @param numberTextureCordinates The number of texture coordinates.
         * @param numberTriangles The number of triangles.
         * @param numberGlCommands The number of gl commands.
         * @param numberFrames The number of frames included in the model.
         * @param offsetSkins The offset address to the skins.
         * @param offsetSt The offset address to the texture coordinates.
         * @param offsetTriangles The offset address to the trinagles.
         * @param offsetFrames The offset address to the frames.
         * @param offsetGlCommands The offset address to the gl commands.
         * @param offsetEnd The offset address to the end.
         */
        typedef struct Md2Header_t {
            char magicNumber[4];
            int md2Version;
            int textureWidth;
            int textureHeight;
            int frameSize;
            int numberTextures;
            int numberVertices;
            int numberTextureCordinates;
            int numberTriangles;
            int numberGlCommands;
            int numberFrames;
            int offsetSkins;
            int offsetSt;
            int offsetTriangles;
            int offsetFrames;
            int offsetGlCommands;
            int offsetEnd;
        } Md2Header;

        /**
         * The md2 frame header information structure.
         *
         * @param scale The scaling information for each of the
         * coordinates.
         * @param translate The translation value for each of the coordinates.
         * @param name The name used to identify the frame.
         */
        typedef struct Md2FrameHeader_t {
            float scale[3];
            float translate[3];
            char name[16];
        } Md2FrameHeader;

        /**
         * The md2 vertex contents used for operation in the vertex.
         *
         * @param vertex The vertex information.
         * @param lightNormalIndex The index for normal vector calculation.
         */
        typedef struct Md2VertexContents_t {
            unsigned char vertex[3];
            unsigned char lightNormalIndex;
        } Md2VertexContents;

        /**
         * The md2 texture information that contains
         * mapping information for the texture.
         *
         * @param vertexTextureX The x coordinate texture value.
         * @param vertexTextureY The y coordinate texture value.
         * @parma vertexIndex The index for the vertex.
         */
        typedef struct Md2VertexTextureInformation_t {
            float vertexTextureX;
            float vertexTextureY;
            int vertexIndex;
        } Md2VertexTextureInformation;

        /**
         * Class that represents an md2 (quake 2) frame, the contents
         * include coordinates list, vertex list, gl commands and mesh list.
         *
         * @see wikipedia - http://en.wikipedia.org/wiki/MD2_(file_format)
         */
        class Md2Frame {
            private:

            public:
                std::vector<float> coordinatesList;
                std::vector<float *> vertexList;
                std::vector<structures::Mesh_t *> meshList;
                structures::Frame_t frame;
                int vertexCount;
                int meshCount;

                Md2Frame();
                ~Md2Frame();
                void generateVertexList();
                void generateMeshList(const std::vector<void *> &glCommandsList);
                void countMeshes(const std::vector<void *> &glCommandsList);
                void compileDisplayList();
                structures::Frame_t *getFrame();
                void cleanVertextList();
                void cleanMeshList();
        };

        /**
         * The md2 importer class.
         * Allows the importing to the mariachi engine of the quake 2 models.
         *
         * @see wikipedia - http://en.wikipedia.org/wiki/MD2_(file_format)
         */
        class Md2Importer : public ModelImporter {
            private:
                std::vector<structures::Frame_t *> framesList;
                std::vector<Md2Frame *> md2FramesList;
                std::vector<void *> glCommandsList;
                int frameCount;

                inline void generateCoordinatesList(Md2Header_t *md2Header, char *md2Contents);
                inline void generateGlCommandsList(Md2Header_t *md2Header, char *md2Contents);
                Md2Frame *getMainMd2Frame();

            public:
                Md2Importer();
                ~Md2Importer();
                void generateModel(const std::string &filePath);
                void generateVertexList();
                void generateMeshList();
                void generateFrameList();
                nodes::ModelNode *getModelNode();
                nodes::ActorNode *getActorNode();
                void cleanModel();
                void cleanMd2FrameList();
        };
    }
}
