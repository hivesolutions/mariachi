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

#include "stdafx.h"

#include "bullet_physics_engine_cube_solid.h"

using namespace mariachi::physics;
using namespace mariachi::structures;

BulletPhysicsEngineCubeSolid::BulletPhysicsEngineCubeSolid() : BulletPhysicsEngineCollisionSolid(), CubeSolid() {

}

BulletPhysicsEngineCubeSolid::~BulletPhysicsEngineCubeSolid() {
}

void BulletPhysicsEngineCubeSolid::setBoundingBox(const Box3d_t &boundingBox) {
    CubeSolid::setBoundingBox(boundingBox);

    btScalar width = fabs(boundingBox.x2 - boundingBox.x1);
    btScalar height = fabs(boundingBox.y2 - boundingBox.y1);
    btScalar depth = fabs(boundingBox.z2 - boundingBox.z1);

    btVector3 boxExtents(width, height, depth);

    this->collisionShape = new btBoxShape(boxExtents);
}
