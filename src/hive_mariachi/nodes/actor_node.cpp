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

#include "actor_node.h"

using namespace mariachi::nodes;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
ActorNode::ActorNode() : ModelNode() {
    this->initRenderable();
    this->initCurrentFrame();
}

ActorNode::ActorNode(const std::string &name) : ModelNode(name) {
    this->initRenderable();
    this->initCurrentFrame();
}

/**
 * Destructor of the class.
 */
ActorNode::~ActorNode() {
}

inline void ActorNode::initRenderable() {
    this->renderable = true;
}

inline void ActorNode::initCurrentFrame() {
    this->currentFrame = 0;
}

inline Frame_t *ActorNode::getCurrentFrame() {
    return (*this->frameList)[this->currentFrame];
}

inline void ActorNode::incrementFrame() {
    // retrieves the frame list size
    unsigned int frameListSize = this->frameList->size();

    // in case the current frame is the last one
    // of the animation sequence
    if(this->currentFrame == frameListSize - 1) {
        // resets the current frame
        this->currentFrame = 0;
    } else {
        // increments the current frame
        this->currentFrame++;
    }
}

std::vector<Mesh_t *> *ActorNode::getMeshList() {
    // retrieves the current frame
    Frame_t *currentFrame = this->getCurrentFrame();

    // returns the current frame mesh list
    return currentFrame->meshList;
}

std::vector<Frame_t *> *ActorNode::getFrameList() {
    return this->frameList;
}

void ActorNode::setFrameList(std::vector<Frame_t *> *frameList) {
    this->frameList = frameList;
}

void ActorNode::updateFrame() {
    this->incrementFrame();
}

void ActorNode::loop(const std::string &animationName) {

}
