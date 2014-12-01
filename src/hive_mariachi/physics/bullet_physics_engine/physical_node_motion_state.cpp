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

#include "physical_node_motion_state.h"

using namespace mariachi;
using namespace mariachi::nodes;
using namespace mariachi::physics;
using namespace mariachi::structures;

PhysicalNodeMotionState::PhysicalNodeMotionState(const btTransform &worldTransform) {
    this->initWorldTransform(worldTransform);
}

PhysicalNodeMotionState::PhysicalNodeMotionState(const btTransform &worldTransform, PhysicalNode *physicalNode) {
    this->initWorldTransform(worldTransform);
    this->initPhysicalNode(physicalNode);
}

PhysicalNodeMotionState::~PhysicalNodeMotionState() {
}

inline void PhysicalNodeMotionState::initWorldTransform(const btTransform &worldTransform) {
    this->worldTransform = worldTransform;
}

inline void PhysicalNodeMotionState::initPhysicalNode(PhysicalNode *physicalNode) {
    this->physicalNode = physicalNode;
}

void PhysicalNodeMotionState::getWorldTransform(btTransform &worldTransform) const {
    worldTransform = this->worldTransform;
}

void PhysicalNodeMotionState::setWorldTransform(const btTransform &worldTransform) {
    // returns silently if node not set
    if(this->physicalNode == NULL) {
        return;
    }

    // in case the physical node allows physical positioning
    if(this->physicalNode->getPhysicalPositionEnabled()) {
        // retrieves the bullet position
        btVector3 positionBullet = worldTransform.getOrigin();

        // creates the position from the bullet position
        Coordinate3d_t position = { positionBullet.x(), positionBullet.y(), positionBullet.z() };

        // sets the position in the cube node
        this->physicalNode->setPosition(position);
    }

    // in case the physical node allows physical rotation
    if(this->physicalNode->getPhysicalRotationEnabled()) {
        // retrieves the bullet rotation
        btQuaternion rotationBullet = worldTransform.getRotation();

        // retrieves the rotation angle
        float angle = 2.0f * acos(rotationBullet.w()) * 180.0f / (float) M_PI;

        // retrieves the rotation axis vector
        float norm = sqrt(pow(rotationBullet.x(), 2.0f) + pow(rotationBullet.y(), 2.0f) + pow(rotationBullet.z(), 2.0f));
        float normalizedX = rotationBullet.x() / norm;
        float normalizedY = rotationBullet.y() / norm;
        float normalizedZ = rotationBullet.z() / norm;

        // creates the rotation for the physical node
        Rotation3d_t rotation = { angle, normalizedX, normalizedY, normalizedZ };

        // sets the rotation in the physical node
        this->physicalNode->setRotation(rotation);
    }
}

/**
 * Updates bullet's world transform to reflect the new position. Useful for kinematic bodies.
 */
void PhysicalNodeMotionState::setPosition(const Coordinate3d_t &position) {
    // creates the position vector for the specified position
    btVector3 positionVector(position.x, position.y, position.z);

    // sets the position in bullet's world transform
    this->worldTransform.setOrigin(positionVector);
}

PhysicalNode *PhysicalNodeMotionState::getPhysicalNode() {
    return this->physicalNode;
}

void PhysicalNodeMotionState::setPhysicalNode(PhysicalNode *physicalNode) {
    this->physicalNode = physicalNode;
}
