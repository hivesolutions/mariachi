// Hive Mariachi Engine
// Copyright (c) 2008-2019 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2019 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../structures/box.h"
#include "../structures/position.h"

namespace mariachi {
    namespace util {
        class BoxUtil {
            private:

            public:
                static structures::Box3d_t createBox(float width, float height, float depth);
                static structures::Box3d_t createBox(float x1, float y1, float z1, float x2, float y2, float z2);
                static bool overlaps(const structures::Box3d_t &box1, const structures::Box3d_t &box2);
                static bool containsPoint(const structures::Box3d_t &box, const structures::Coordinate3d_t &point);
        };
    }
}
