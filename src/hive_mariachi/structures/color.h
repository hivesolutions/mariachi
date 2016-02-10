// Hive Mariachi Engine
// Copyright (c) 2008-2016 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2016 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

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
