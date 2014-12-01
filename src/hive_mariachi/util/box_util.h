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
