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

#include "vector_util.h"

using namespace mariachi::util;
using namespace mariachi::structures;

const Coordinate3d_t VectorUtil::NULL_VECTOR = { 0.0f, 0.0f, 0.0f };

Coordinate3d_t VectorUtil::normalize(const Coordinate3d_t &vector) {
    // the normalized vector to compute
    Coordinate3d_t normalizedVector;

    // calculates the vector's magnitude
    float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    // normalizes the vector
    if(magnitude) {
        normalizedVector.x = vector.x / magnitude;
        normalizedVector.y = vector.y / magnitude;
        normalizedVector.z = vector.z / magnitude;
    } else {
        normalizedVector.x = 0.0f;
        normalizedVector.y = 0.0f;
        normalizedVector.z = 0.0f;
    }

    // returns the normalized vector
    return normalizedVector;
}

Coordinate3d_t VectorUtil::crossProduct(const Coordinate3d_t &vector1, const Coordinate3d_t &vector2) {
    Coordinate3d_t crossProductVector;

    // calculates the cross product
    crossProductVector.x = vector1.y * vector2.z - vector1.z * vector2.y;
    crossProductVector.y = -vector1.x * vector2.z + vector1.z * vector2.x;
    crossProductVector.z = vector1.x * vector2.y - vector1.y * vector2.x;

    // returns the cross product result
    return crossProductVector;
}

Coordinate3d_t VectorUtil::add(const Coordinate3d_t &vector1, const Coordinate3d_t &vector2) {
    Coordinate3d_t vectorAddition;

    vectorAddition.x = vector1.x + vector2.x;
    vectorAddition.y = vector1.y + vector2.y;
    vectorAddition.z = vector1.z + vector2.z;

    return vectorAddition;
}

Coordinate3d_t VectorUtil::subtract(const Coordinate3d_t &vector1, const Coordinate3d_t &vector2) {
    Coordinate3d_t vectorSubtraction;

    vectorSubtraction.x = vector1.x - vector2.x;
    vectorSubtraction.y = vector1.y - vector2.y;
    vectorSubtraction.z = vector1.z - vector2.z;

    return vectorSubtraction;
}
