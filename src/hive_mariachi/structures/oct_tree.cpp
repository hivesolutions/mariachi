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

#include "stdafx.h"

#include "../util/util.h"
#include "oct_tree.h"

using namespace mariachi::util;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
OctTree::OctTree() {
}

/**
 * Constructor of the class with bounding box.
 */
OctTree::OctTree(Box3d_t boundingBox) {
    // creates the root node using the provided bounding box
    this->rootNode = new OctTreeNode(boundingBox);
}

/**
 * Constructor of the class with bounding box extents.
 */
OctTree::OctTree(float boxWidth, float boxHeight, float boxDepth) {
    // creates the root node using the provided dimensions
    this->rootNode = new OctTreeNode(boxWidth, boxHeight, boxDepth);
}

/**
 * Destructor of the class.
 */
OctTree::~OctTree() {
    // destroys the root node
    delete this->rootNode;
}

/**
 * Inserts an element into the oct tree.
 * Delegates the insertion process to the root node.
 *
 * @param element The element to insert in the oct tree.
 * @param elementBoundingBox The box bounding the inserted element.
 */
void OctTree::insertElementBox(void *element, Box3d_t *elementBoundingBox) {
    this->rootNode->insertElementBox(element, elementBoundingBox);
}

/**
 * Retrieves the elements bounded by the provided query box.
 * Delegates the retrieval process to the root node.
 *
 * @param queryBox The box for which contained elements are retrieved.
 * @return The elements bounded by the provided box.
 */
std::vector<void *> OctTree::getBoxElements(Box3d_t *queryBox) {
    std::vector<void *> elements = this->rootNode->getBoxElements(queryBox);
    return elements;
}
