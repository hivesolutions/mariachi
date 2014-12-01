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

#include "../structures/position.h"
#include "model_importer.h"

/**
 * The md3 header size.
 */
#define MD3_HEADER_SIZE 112

/**
 * The md3 frame header size.
 */
#define MD3_FRAME_HEADER_SIZE 56

namespace mariachi {
    namespace importers {
        /**
         * The md3 header structure, containing information about
         * md3 metadata.
         *
         * @param magicNumber The md3 magic number (IDP3).
         * @param md3Version The version of the md3 implementation.
         * @param name The name of the model.
         * @param flags The flags that control the behaviour of the model.
         * @param numberFrames The number of frames in the model.
         * @param numberTags The number of tags of the model.
         * @param numberSurfaces The number of surfaces.
         * @param numberSkins The number of skins of the model.
         * @param offsetFrames The offset address to the frames.
         * @param offsetTags The offset address to the tags.
         * @param offsetSurfaces The offset address to the frames.
         * @param offsetEnd The offset address to the end.
         */
        typedef struct Md3Header_t {
            char magicNumber[4];
            int md3Version;
            char name[64];
            int flags;
            int numberFrames;
            int numberTags;
            int numberSurfaces;
            int numberSkins;
            int offsetFrames;
            int offsetTags;
            int offsetSurfaces;
            int offsetEnd;
        } Md3Header;

        /**
         * The md3 frame header information structure.
         *
         * @param minimumBounds The first corner of the bounding box.
         * @param maximumBounds The second corner of the bounding box.
         * @param localOrigin The position to be used as the origin
         * position for the model (usually 0.0, 0.0, 0.0).
         * @param radius The radius of the frame.
         * @param name The name used to identify the frame.
         */
        typedef struct Md3FrameHeader_t {
            structures::Coordinate3d_t minimumBounds;
            structures::Coordinate3d_t maximumBounds;
            structures::Coordinate3d_t localOrigin;
            float radius;
            char name[16];
        } Md3FrameHeader;

        /**
         * The md3 tag header information structure.
         *
         * @param name The name used to identify the tag.
         * @param origin The coordinates of tag object.
         * @param axis The 3x3 rotation matrix associated with the tag.
         */
        typedef struct Md3TagHeader_t {
            char name[64];
            structures::Coordinate3d_t origin;
            structures::Coordinate3d_t axis[3];
        } Md3TagHeader;

        typedef struct Md3Shader_t {
            char name[64];
            int shaderIndex;
        } Md3Shader;

        typedef struct Md3SurfaceHeader_t {
            int indentation;
            char name[64];
            int flags;
            int numberFrames;
            int numberShaders;
            int numberVertices;
            int numberTriangles;
            int offsetShaders;
            int offsetStart;
            int offsetXYZNormal;
            int offsetEnd;
        } Md3SurfaceHeader;

        /**
         * Class that represents an md3 (quake 3) frame.
         *
         * @see wikipedia -http://en.wikipedia.org/wiki/MD3_(file_format)
         */
        class Md3Frame {
            private:

            public:
                Md3Frame();
                ~Md3Frame();
        };

        /**
         * Class that represents an md3 (quake 3) frame, the contents
         * include coordinates list, vertex list and mesh list.
         *
         * @see wikipedia - http://en.wikipedia.org/wiki/MD3_(file_format)
         */
        class Md3Importer : public ModelImporter {
            private:
                std::vector<Md3Frame *> md3FramesList;
                int frameCount;
                int tagCount;
                int surfaceCount;

                inline void generateFramesList(Md3Header_t *md3Header, char *md3Contents);
                inline void generateTagsList(Md3Header_t *md3Header, char *md3Contents);
                inline void generateSurfacesList(Md3Header_t *md3Header, char *md3Contents);

            public:
                Md3Importer();
                ~Md3Importer();
                void generateModel(const std::string &filePath);
        };
    }
}
