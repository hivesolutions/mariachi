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

// __author__    = Tiago Silva <tsilva@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "geometry_util.h"

using namespace mariachi::util;
using namespace mariachi::structures;

float GeometryUtil::getEuclidianDistance(Coordinate2d_t *firstCoordinate, Coordinate2d_t *secondCoordinate) {
    return sqrt(pow(secondCoordinate->x - firstCoordinate->x, 2) + pow(secondCoordinate->y - firstCoordinate->y, 2));
}

float GeometryUtil::getEuclidianDistance(Coordinate3d_t *firstCoordinate, Coordinate3d_t *secondCoordinate) {
    return sqrt(pow(secondCoordinate->x - firstCoordinate->x, 2) + pow(secondCoordinate->y - firstCoordinate->y, 2) + pow(secondCoordinate->z - firstCoordinate->z, 2));
}

float GeometryUtil::getManhattanDistance(Coordinate2d_t *firstCoordinate, Coordinate2d_t *secondCoordinate) {
    return fabs(secondCoordinate->x - firstCoordinate->x) + fabs(secondCoordinate->y - firstCoordinate->y);
}

float GeometryUtil::getManhattanDistance(Coordinate3d_t *firstCoordinate, Coordinate3d_t *secondCoordinate) {
    return fabs(secondCoordinate->x - firstCoordinate->x) + fabs(secondCoordinate->y - firstCoordinate->y) + fabs(secondCoordinate->z - firstCoordinate->z);
}
