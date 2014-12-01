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

#include "../structures/rotation.h"
#include "../util/vector_util.h"

#include "camera_node.h"

using namespace mariachi::util;
using namespace mariachi::nodes;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
CameraNode::CameraNode() : LensNode() {
    // initializes the auto tracking status
    this->initializeAutoTracking();

    // initializes the auto following status
    this->initializeAutoFollowing();
}

CameraNode::CameraNode(const std::string &name) : LensNode(name) {
    // initializes the auto tracking status
    this->initializeAutoTracking();

    // initializes the auto following status
    this->initializeAutoFollowing();
}

inline void CameraNode::initializeAutoTracking() {
    // initializes the auto track target node
    this->autoTrackTargetNode = NULL;

    // initializes the auto track offset
    this->autoTrackOffset = VectorUtil::NULL_VECTOR;
}

inline void CameraNode::initializeAutoFollowing() {
    // initializes the auto track target node
    this->autoFollowTargetNode = NULL;

    // initializes the auto follow offset
    this->autoFollowOffset = VectorUtil::NULL_VECTOR;
}

/**
 * Destructor of the class.
 */
CameraNode::~CameraNode() {
}

void CameraNode::lookAt(const Coordinate3d_t &targetPoint) {
    // the direction vector is the difference between the eye position and the target point
    Coordinate3d_t direction = VectorUtil::subtract(this->getPosition(), targetPoint);

    // sets the direction
    this->setDirection(direction);
}

void CameraNode::lookAt(float x, float y, float z) {
    // creates a target point coordinate struct
    Coordinate3d_t targetPoint = { x, y, z };

    // looks at the 3d point
    this->lookAt(targetPoint);
}

inline bool CameraNode::isAutoTracking() {
    // in case the auto track target is set
    // the camera is auto tracking a target
    return this->autoTrackTargetNode != NULL;
}

inline bool CameraNode::isAutoFollowing() {
    // in case the auto track target is set
    // the camera is auto tracking a target
    return this->autoFollowTargetNode != NULL;
}

void CameraNode::enableAutoTracking(CubeNode *targetNode) {
    // enables auto tracking by setting the auto track target node
    this->autoTrackTargetNode = targetNode;
}

void CameraNode::enableAutoTracking(CubeNode *targetNode, Coordinate3d_t &offset) {
    // enables auto tracking by setting the auto track target node
    this->autoTrackTargetNode = targetNode;

    // sets the auto track offset
    this->autoTrackOffset = offset;
}

void CameraNode::disableAutoTracking() {
    // disables auto tracking by resetting the auto track target node
    this->autoTrackTargetNode = NULL;
}

void CameraNode::enableAutoFollowing(CubeNode *targetNode) {
    // sets the follow target node
    this->autoFollowTargetNode = targetNode;
}

void CameraNode::enableAutoFollowing(CubeNode *targetNode, Coordinate3d_t &offset) {
    // sets the follow target node
    this->autoFollowTargetNode = targetNode;

    // sets the auto follow offset
    this->autoFollowOffset = offset;
}

void CameraNode::disableAutoFollowing() {
    // resets the auto follow target node
    this->autoFollowTargetNode = NULL;
}

void CameraNode::setDirection(const Coordinate3d_t &direction) {
    Rotation3d_t rotation;
    Coordinate3d_t sideVector;
    Coordinate3d_t realUpVector;

    // normalizes the direction vector
    Coordinate3d_t normalizedDirection = VectorUtil::normalize(direction);

    // side vector = up cross direction
    sideVector = VectorUtil::crossProduct(this->upVector, normalizedDirection);

    // recompute up = direction cross side
    realUpVector = VectorUtil::crossProduct(normalizedDirection, sideVector);

    // normalize the side vector
    Coordinate3d_t normalizedSideVector = VectorUtil::normalize(sideVector);

    // normalize the up vector
    Coordinate3d_t normalizedRealUpVector = VectorUtil::normalize(realUpVector);

    // determines the rotation angle
    rotation.angle = acos((normalizedSideVector.x + normalizedRealUpVector.y + normalizedDirection.z - 1.0f) / 2.0f) * 180.0f / (float) M_PI;

    // @todo: account for singularities

    // determines the magnitude
    float magnitude = sqrt((normalizedDirection.y - normalizedRealUpVector.z) * (normalizedDirection.y - normalizedRealUpVector.z) + (normalizedSideVector.z - normalizedDirection.x) * (normalizedSideVector.z - normalizedDirection.x) + (normalizedRealUpVector.x - normalizedSideVector.y) * (normalizedRealUpVector.x - normalizedSideVector.y));

    // determines the rotation axis
    rotation.x = (normalizedDirection.y - normalizedRealUpVector.z) / magnitude;
    rotation.y = (normalizedSideVector.z - normalizedDirection.x) / magnitude;
    rotation.z = (normalizedRealUpVector.x - normalizedSideVector.y) / magnitude;

    // sets the axis angle representation of the rotation in the node rotation
    this->setRotation(rotation);
}

void CameraNode::setDirection(float x, float y, float z) {
    Coordinate3d_t direction = { x, y, z };
    this->setDirection(direction);
}

Coordinate3d_t &CameraNode::getUpVector() {
    return this->upVector;
}

void CameraNode::setUpVector(const Coordinate3d_t &upVector) {
    this->upVector = upVector;
}

void CameraNode::setUpVector(float x, float y, float z) {
    Coordinate3d_t upVector = { x, y, z };
    this->setUpVector(upVector);
}

void CameraNode::_autoTrack() {
    // in case an auto track target is defined
    if(this->isAutoTracking()) {
        // adds the offset to the target position
        Coordinate3d_t autoTrackTargetPositionOffset = VectorUtil::add(this->autoTrackTargetNode->getPosition(), this->autoTrackOffset);

        // looks at the target using the configured offset
        this->lookAt(autoTrackTargetPositionOffset);
    }
}

void CameraNode::_autoFollow() {
    // in case an auto track target is defined
    if(this->isAutoFollowing()) {
        // adds the offset to the target position
        Coordinate3d_t autoFollowTargetPositionOffset = VectorUtil::add(this->autoFollowTargetNode->getPosition(), this->autoFollowOffset);

        // sets the camera at the target position
        this->setPosition(autoFollowTargetPositionOffset);
    }
}
