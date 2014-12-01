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

// __author__    = Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#define RED_COLOR { 255, 0, 0 }
#define RED_COLOR_FLOAT { 1.0, 0.0, 0.0 }
#define GREEN_COLOR { 0, 255, 0 }
#define GREEN_COLOR_FLOAT { 0.0, 1.0, 0.0 }
#define BLUE_COLOR { 0, 0, 255 }
#define BLUE_COLOR_FLOAT { 0.0, 0.0, 1.0 }

namespace mariachi {
    namespace structures {
        typedef struct CharColor_t {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
        } CharColor;

        typedef struct CharColorAlpha_t {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
            unsigned char alpha;
        } CharColorAlpha;

        typedef struct IntColor_t {
            unsigned int red;
            unsigned int green;
            unsigned int blue;
        } IntColor;

        typedef struct IntColorAlpha_t {
            unsigned int red;
            unsigned int green;
            unsigned int blue;
            unsigned int alpha;
        } IntColorAlpha;

        typedef struct FloatColor_t {
            float red;
            float green;
            float blue;
        } FloatColor;

        typedef struct FloatColorAlpha_t {
            float red;
            float green;
            float blue;
            float alpha;
        } FloatColorAlpha;
    }
}
