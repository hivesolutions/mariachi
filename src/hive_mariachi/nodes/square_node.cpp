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

#include "square_node.h"

using namespace mariachi::nodes;
using namespace mariachi::structures;

SquareNode::SquareNode() : Node() {
    // intializes the position reference
    this->initPositionReference();
}

SquareNode::SquareNode(const std::string &name) : Node(name) {
    // intializes the position reference
    this->initPositionReference();
}

SquareNode::~SquareNode() {
}

inline void SquareNode::initPositionReference() {
    this->topLeftPositionDirty = true;
    this->positionReference = TOP_LEFT_REFERENCE_POSITION;
}

Coordinate2d_t &SquareNode::getTopLeftPosition() {
    // in case the top left position is not dirty
    if(!this->topLeftPositionDirty) {
        // returns the top left position
        return this->topLeftPosition;
    }

    // switches over the position reference
    switch(this->positionReference) {
        case TOP_LEFT_REFERENCE_POSITION:
            this->topLeftPosition = this->position;

            break;

        case CENTER_REFERENCE_POSITION:
            this->topLeftPosition.x = this->position.x - (this->size.width / 2.0f);
            this->topLeftPosition.y = this->position.y - (this->size.height / 2.0f);

            break;
    }

    // unsets the top left position dirty flag
    this->topLeftPositionDirty = false;

    // returns the top left position
    return this->topLeftPosition;
}

Coordinate2d_t &SquareNode::getPosition() {
    return this->position;
}

void SquareNode::setPosition(Coordinate2d_t &position) {
    this->position = position;
    this->topLeftPositionDirty = true;
}

void SquareNode::setPosition(float x, float y) {
    Coordinate2d_t position = { x, y } ;

    this->position = position;
    this->topLeftPositionDirty = true;
}

FloatSize2d_t &SquareNode::getSize() {
    return this->size;
}

void SquareNode::setSize(FloatSize2d_t &size) {
    this->size = size;
    this->topLeftPositionDirty = true;
}

void SquareNode::setSize(float width, float height) {
    FloatSize2d_t size = { width, height };

    this->size = size;
    this->topLeftPositionDirty = true;
}

PositionReferenceType_t SquareNode::getPositionReference() {
    return this->positionReference;
}

void SquareNode::setPositionReference(PositionReferenceType_t positionReference) {
    this->positionReference = positionReference;
    this->topLeftPositionDirty = true;
}
