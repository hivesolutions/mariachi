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

#include "../nodes/physical_node.h"

/**
 * The size of the collision point list.
 */
#define COLLISION_POINT_LIST_SIZE 4

namespace mariachi {
    namespace structures {
        /**
         * Represents a collision point in 3d space.
         *
         * @param positionFirstPhysicalNode The collision position on the first physical node.
         * @param positionSecondPhysicalNode The collision position on the second physical node.
         * @param normalSecondPhysicalNode The normal on the second physical node.
         */
        typedef struct CollisionPoint3d_t {
            Coordinate3d_t positionFirstPhysicalNode;
            Coordinate3d_t positionSecondPhysicalNode;
            Coordinate3d_t normalSecondPhysicalNode;
        } CollisionPoint3d;

        /**
         * Represents a collision in 3d space.
         *
         * @param collisionPointList The list of collision points in this collision.
         * @param firstPhysicalNode The first physical node which collided.
         * @param secondPhysicalNode The second physical node which collided.
         */
        typedef struct Collision3d_t {
            CollisionPoint3d collisionPointList[COLLISION_POINT_LIST_SIZE];
            nodes::PhysicalNode *firstPhysicalNode;
            nodes::PhysicalNode *secondPhysicalNode;
        } Collision3d;
    }
}
