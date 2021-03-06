// Hive Mariachi Engine
// Copyright (c) 2008-2020 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#include "stdafx.h"

#include "ui_component_node.h"

using namespace mariachi::ui;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
ComponentNode::ComponentNode() : UiNode() {
    this->initRenderable();
}

ComponentNode::ComponentNode(const std::string &name) : UiNode(name) {
    this->initRenderable();
}

/**
 * Destructor of the class.
 */
ComponentNode::~ComponentNode() {
}

inline void ComponentNode::initRenderable() {
    this->renderable = true;
}

/**
 * Retrieves the color.
 *
 * @return The color.
 */
FloatColor_t &ComponentNode::getColor() {
    return this->color;
}

/**
 * Sets the color.
 *
 * @param color The color.
 */
void ComponentNode::setColor(const FloatColor_t &color) {
    this->color = color;
}

/**
 * Retrieves the texture.
 *
 * @return The texture.
 */
Texture *ComponentNode::getTexture() {
    return this->texture;
}

/**
 * Sets the texture.
 *
 * @param texture The texture.
 */
void ComponentNode::setTexture(Texture *texture) {
    this->texture = texture;
}
