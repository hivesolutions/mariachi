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

#include "stdafx.h"

#include "box_util.h"

using namespace mariachi::util;
using namespace mariachi::structures;

/**
 * Creates a new a box for the provided extents, and centered in the origin of the world coordinates.
 */
Box3d_t BoxUtil::createBox(float width, float height, float depth) {
    // computes half the width
    float halfWidth = width / 2.0f;

    // computes half the width
    float halfHeight = height / 2.0f;

    // computes half the depth
    float halfDepth = depth / 2.0f;

    // creates a new box
    Box3d_t box = { -halfWidth, -halfHeight, -halfDepth, halfWidth, halfHeight, halfDepth };

    // returns the created box
    return box;
}

/**
 * Creates a new a box box for the provided coordinates.
 */
Box3d_t BoxUtil::createBox(float x1, float y1, float z1, float x2, float y2, float z2) {
    // creates a new box
    Box3d_t box = { x1, y1, z1, x2, y2, z2 };

    // returns the created box
    return box;
}

/**
 * Tests two boxes for overlapping.
 */
bool BoxUtil::overlaps(const Box3d_t &box1, const Box3d_t &box2) {
    return box1.x1 == box2.x1 && box1.x2 == box2.x2 && box1.y1 == box2.y1 && box1.y2 == box2.y2 && box1.z1 == box2.z1 && box1.z2 == box2.z2;
}

/**
 * Determines if the specified box contains the provided point.
 */
bool BoxUtil::containsPoint(const Box3d_t &box, const Coordinate3d_t &point) {
    return point.x >= box.x1 && point.y >= box.y1 && point.z >= box.z1 && point.x <= box.x2 && point.y <= box.y2 && point.z <= box.z2;
}
