// Hive Mariachi Engine
// Copyright (c) 2008-2017 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2017 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#include "stdafx.h"

#include "model_node.h"

using namespace mariachi::nodes;
using namespace mariachi::structures;

ModelNode::ModelNode() : PhysicalNode() {
    this->initRenderable();
}

ModelNode::ModelNode(const std::string &name) : PhysicalNode(name) {
    this->initRenderable();
}

ModelNode::~ModelNode() {
    delete this->texture;
}

inline void ModelNode::initRenderable() {
    this->renderable = true;
}

std::vector<Mesh_t *> *ModelNode::getMeshList() {
    return this->meshList;
}

void ModelNode::setMeshList(std::vector<Mesh_t *> *meshList) {
    this->meshList = meshList;
}

Texture *ModelNode::getTexture() {
    return this->texture;
}

void ModelNode::setTexture(Texture *texture) {
    this->texture = texture;
}
