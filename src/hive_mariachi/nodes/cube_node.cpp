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

#include "cube_node.h"

using namespace mariachi::nodes;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
CubeNode::CubeNode() : Node() {
    // initialize the cube node's transforms (translation, scale and rotation)
    this->initTransforms();
}

CubeNode::CubeNode(const std::string &name) : Node(name) {
    // initialize the cube node's transforms (translation, scale and rotation)
    this->initTransforms();
}

/**
 * Destructor of the class.
 */
CubeNode::~CubeNode() {
}

inline void CubeNode::initTransforms() {
    // creats the origin position
    Coordinate3d_t initialPosition = { 0.0f, 0.0f, 0.0f };

    // initializes the node's position
    this->position = initialPosition;

    // creates an empty rotation
    Rotation3d_t initialRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

    // initializes the node's rotation
    this->rotation = initialRotation;

    // creates the identity scale
    Coordinate3d_t initialScale = { 1.0f, 1.0f, 1.0f };

    // initializes the node's scale
    this->scale = initialScale;
}

Coordinate3d_t &CubeNode::getPosition() {
    return this->position;
}

void CubeNode::setPosition(Coordinate3d_t &position) {
    this->position = position;
}

void CubeNode::setPosition(float x, float y, float z) {
    Coordinate3d_t position = {x, y, z};

    this->position = position;
}

Rotation3d_t &CubeNode::getRotation() {
    return this->rotation;
}

void CubeNode::setRotation(Rotation3d_t &rotation) {
    this->rotation = rotation;
}

void CubeNode::setRotation(float angle, float x, float y, float z) {
    Rotation3d_t rotation = {angle, x, y, z};

    this->rotation = rotation;
}

/**
 * Defines the rotation in terms of euler angles, i.e.,
 * a sequence of rotations across the three axis.
 * @see wikipedia - http://en.wikipedia.org/wiki/Euler_angles
 * @see EuclideanSpace - http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToAngle/index.htm
 */
void CubeNode::setRotation(float xAxisDegrees, float yAxisDegrees, float zAxisDegrees) {
    // converts the angles to radians
    float xAxisRadians = xAxisDegrees / 180.0f * (float) M_PI;
    float yAxisRadians = yAxisDegrees / 180.0f * (float) M_PI;
    float zAxisRadians = zAxisDegrees / 180.0f * (float) M_PI;

    float c1 = cos(yAxisRadians/2);
    float s1 = sin(yAxisRadians/2);
    float c2 = cos(zAxisRadians/2);
    float s2 = sin(zAxisRadians/2);
    float c3 = cos(xAxisRadians/2);
    float s3 = sin(xAxisRadians/2);

    float c1c2 = c1*c2;
    float s1s2 = s1*s2;

    // determines the rotation w of the quaternion
    float w = c1c2*c3 - s1s2*s3;

    // determines the rotation vector of the quaternion
    float x = c1c2*s3 + s1s2*c3;
    float y = s1*c2*c3 + c1*s2*s3;
    float z = c1*s2*c3 - s1*c2*s3;

    // determines the angle for the rotation
    float angleRadians = 2 * acos(w);
    float angleDegrees = angleRadians / (float) M_PI * 180.0f;

    // determines the rotation vector norm
    float norm = x*x+y*y+z*z;

    // normalizes the vector
    if (norm > 0) {
        norm = sqrt(norm);
        x /= norm;
        y /= norm;
        z /= norm;
    }
    // avoids division by zero
    else {
        x = 1;
        y = 0;
        z = 0;
    }

    // sets the computed rotation values
    this->setRotation(angleDegrees, x, y, z);
}

Coordinate3d_t &CubeNode::getScale() {
    return this->scale;
}

void CubeNode::setScale(Coordinate3d_t &scale) {
    this->scale = scale;
}

void CubeNode::setScale(float scaleX, float scaleY, float scaleZ) {
    Coordinate3d_t scale = {scaleX, scaleY, scaleZ};

    this->scale = scale;
}
