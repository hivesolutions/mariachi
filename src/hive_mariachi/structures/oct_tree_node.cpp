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
#include "oct_tree_node.h"

using namespace mariachi::util;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
OctTreeNode::OctTreeNode() {
}

/**
 * Constructor of the class with bounding box.
 */
OctTreeNode::OctTreeNode(Box3d_t boundingBox) {
    // copies the provided bounding box
    this->boundingBox = boundingBox;
}

/**
 * Constructor of the class with bounding box extents.
 */
OctTreeNode::OctTreeNode(float boxWidth, float boxHeight, float boxDepth) {
    // updates the bounding box limits
    this->boundingBox.x1 = 0.0;
    this->boundingBox.y1 = 0.0;
    this->boundingBox.z1 = 0.0;
    this->boundingBox.x2 = boxWidth;
    this->boundingBox.y2 = boxHeight;
    this->boundingBox.z2 = boxDepth;
}

/**
 * Destructor of the class.
 */
OctTreeNode::~OctTreeNode() {
    // caches the child nodes vector size
    unsigned int childNodesSize = this->childNodes.size();

    // for all the child nodes
    for(unsigned int i = 0; i < childNodesSize; i++) {
        // deletes the child node object
        delete this->childNodes[i];
    }
}

/**
 * Inserts an element into the oct tree.
 * Inserts into the element list in case of overlapping (black boxes),
 * recursively tries to insert the element inside the element list of the containing tree nodes
 * (discards white boxes and subdivides grey boxes).
 *
 * @param element The element to insert in the oct tree node.
 * @param elementBoundingBox The box bounding the inserted element.
 */
void OctTreeNode::insertElementBox(void *element, Box3d_t *elementBoundingBox) {
    // initializes the stack of nodes to process
    std::vector<OctTreeNodeBox3d_t> nodesStack;

    // creates the variables for the current values
    OctTreeNodeBox3d_t currentNodeBox;
    OctTreeNode *currentNode;
    Box3d_t currentBox;

    // declares the oct tree node boxes list
    OctTreeNodeBox3d_t nodeBoxes[8];

    // declares the list of oct tree node pointers
    OctTreeNode *nodes[8];

    // declares the list of bounding boxes
    Box3d_t boxes[8];

    // creates a node box with the start element
    nodeBoxes[0].node = this;
    nodeBoxes[0].box = *elementBoundingBox;

    // pushes the first oct tree node onto the stack
    nodesStack.push_back(nodeBoxes[0]);

    // until the stack is not empty
    while(!nodesStack.empty()) {
        // retrieves the current node box structure
        currentNodeBox = nodesStack.back();

        // removes the current node from the stack
        nodesStack.pop_back();

        // retrieves the current node
        currentNode = currentNodeBox.node;

        // retrieves the box with which to bound
        currentBox = currentNodeBox.box;

        // tests for overlapping
        if (BoxUtil::overlaps(currentNode->boundingBox, currentBox)) {
            // adds the element
            currentNode->elements.push_back(element);

            // continues the loop
            continue;
        }

        // in case the octant nodes have not been yet generated
        if (currentNode->childNodes.size() == 0) {
            // checks if the current node can be further dividable
            if(currentNode->isSubdividable()) {
                // generates the child nodes
                currentNode->generateChildNodes();
            }
            // else stops descent and adds the element
            else {
                // adds the element
                currentNode->elements.push_back(element);

                // continues the loop
                continue;
            }
        }

        // retrieves the element's bounding box's vertexes
        Coordinate3d_t leftTopBackPoint = {currentBox.x1, currentBox.y2, currentBox.z1};
        Coordinate3d_t rightTopBackPoint = {currentBox.x2, currentBox.y2, currentBox.z1};
        Coordinate3d_t leftBottomBackPoint = {currentBox.x1, currentBox.y1, currentBox.z1};
        Coordinate3d_t rightBottomBackPoint = {currentBox.x2, currentBox.y1, currentBox.z1};
        Coordinate3d_t leftTopFrontPoint = {currentBox.x1, currentBox.y2, currentBox.z2};
        Coordinate3d_t rightTopFrontPoint = {currentBox.x2, currentBox.y2, currentBox.z2};
        Coordinate3d_t leftBottomFrontPoint = {currentBox.x1, currentBox.y1, currentBox.z2};
        Coordinate3d_t rightBottomFrontPoint = {currentBox.x2, currentBox.y1, currentBox.z2};

        // retrieves the octants for each of the element's bounding box's vertexes
        int leftTopBackPointOctant = currentNode->getPointOctant(leftTopBackPoint);
        int rightTopBackPointOctant = currentNode->getPointOctant(rightTopBackPoint);
        int leftBottomBackPointOctant = currentNode->getPointOctant(leftBottomBackPoint);
        int rightBottomBackPointOctant = currentNode->getPointOctant(rightBottomBackPoint);
        int leftTopFrontPointOctant = currentNode->getPointOctant(leftTopFrontPoint);
        int rightTopFrontPointOctant = currentNode->getPointOctant(rightTopFrontPoint);
        int leftBottomFrontPointOctant = currentNode->getPointOctant(leftBottomFrontPoint);
        int rightBottomFrontPointOctant = currentNode->getPointOctant(rightBottomFrontPoint);

        // in case the element is contained by a single octant
        if(leftTopBackPointOctant == rightTopBackPointOctant &&
           leftTopBackPointOctant == leftBottomBackPointOctant &&
           leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the octant box
            boxes[0] = currentBox;

            // wraps and pushes the node for processing, along with respective box
            this->pushNodeBoxes(1, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element horizontally spans two octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the lower x octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x octant
            nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

            // creates the lower x octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = currentBox.y2;
            boxes[0].z2 = currentBox.z2;

            // creates the higher x octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = currentBox.z2;

            // wraps and pushes the two nodes for processing, along with respective boxes
            this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element vertically spans two octants
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the lower y octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher y octant
            nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

            // creates the lower y octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = currentBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = currentBox.z2;

            // creates the higher y octant box
            boxes[1].x1 = currentBox.x1;
            boxes[1].y1 = nodes[1]->boundingBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = currentBox.z2;

            // wraps and pushes the two nodes for processing, along with respective boxes
            this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans two octants accross the z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower z octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher z octant
            nodes[1] = currentNode->childNodes[leftTopFrontPointOctant];

            // creates the lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = currentBox.x2;
            boxes[0].y2 = currentBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher z octant box
            boxes[1].x1 = currentBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = nodes[1]->boundingBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = currentBox.z2;

            // wraps and pushes the two nodes for processing, along with respective boxes
            this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans four octants across the x and y-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the lower x, lower y octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher x, lower y octant
            nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

            // determines the lower x, higher y octant
            nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x, higher y octant
            nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

            // creates the lower x, lower y octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = currentBox.z2;

            // creates the higher x, lower y octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = nodes[1]->boundingBox.y2;
            boxes[1].z2 = currentBox.z2;

            // creates the lower x, higher y octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = nodes[2]->boundingBox.y1;
            boxes[2].z1 = currentBox.z1;
            boxes[2].x2 = nodes[2]->boundingBox.x2;
            boxes[2].y2 = currentBox.y2;
            boxes[2].z2 = currentBox.z2;

            // creates the higher x, higher y octant box
            boxes[3].x1 = nodes[3]->boundingBox.x1;
            boxes[3].y1 = nodes[3]->boundingBox.y1;
            boxes[3].z1 = currentBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = currentBox.z2;

            // wraps and pushes the four nodes for processing, along with respective boxes
            this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans four octants accross the x and z-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower x, lower z octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x, lower z octant
            nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

            // determines the lower x, higher z octant
            nodes[2] = currentNode->childNodes[leftTopFrontPointOctant];

            // determines the higher x, higher z octant
            nodes[3] = currentNode->childNodes[rightTopFrontPointOctant];

            // creates the lower x, lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = currentBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher x, lower z octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = nodes[1]->boundingBox.z2;

            // creates the lower x, higher z octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = currentBox.y1;
            boxes[2].z1 = nodes[2]->boundingBox.z1;
            boxes[2].x2 = nodes[2]->boundingBox.x2;
            boxes[2].y2 = currentBox.y2;
            boxes[2].z2 = currentBox.z2;

            // creates the higher x, higher z octant box
            boxes[3].x1 = nodes[3]->boundingBox.x1;
            boxes[3].y1 = currentBox.y1;
            boxes[3].z1 = nodes[3]->boundingBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = currentBox.z2;

            // wraps and pushes the four nodes for processing, along with respective boxes
            this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans four octants accross the y and z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower y, lower z octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher y, lower z octant
            nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the lower y, higher z octant
            nodes[2] = currentNode->childNodes[leftBottomFrontPointOctant];

            // determines the higher y, higher z octant
            nodes[3] = currentNode->childNodes[leftTopFrontPointOctant];

            // creates the lower y, lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = currentBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher y, lower z octant box
            boxes[1].x1 = currentBox.x1;
            boxes[1].y1 = nodes[1]->boundingBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = nodes[1]->boundingBox.z2;

            // creates the lower y, higher z octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = currentBox.y1;
            boxes[2].z1 = nodes[2]->boundingBox.z1;
            boxes[2].x2 = currentBox.x2;
            boxes[2].y2 = nodes[2]->boundingBox.y2;
            boxes[2].z2 = currentBox.z2;

            // creates the higher x, higher z octant box
            boxes[3].x1 = currentBox.x1;
            boxes[3].y1 = nodes[3]->boundingBox.y1;
            boxes[3].z1 = nodes[3]->boundingBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = currentBox.z2;

            // wraps and pushes the four nodes for processing, along with respective boxes
            this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans eight octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower x, lower y, lower z octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher x, lower y, lower z octant
            nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

            // determines the lower x, higher y, lower z octant
            nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x, higher y, lower z octant
            nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

            // determines the lower x, lower y, higher z octant
            nodes[4] = currentNode->childNodes[leftBottomFrontPointOctant];

            // determines the higher x, lower y, higher z octant
            nodes[5] = currentNode->childNodes[rightBottomFrontPointOctant];

            // determines the lower x, higher y, higher z octant
            nodes[6] = currentNode->childNodes[leftTopFrontPointOctant];

            // determines the higher x, higher y, higher z octant
            nodes[7] = currentNode->childNodes[rightTopFrontPointOctant];

            // creates the lower x, lower y, lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher x, lower y, lower z octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = nodes[1]->boundingBox.y2;
            boxes[1].z2 = nodes[1]->boundingBox.z2;

            // creates the lower x, higher y, lower z octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = nodes[2]->boundingBox.y1;
            boxes[2].z1 = currentBox.z1;
            boxes[2].x2 = nodes[2]->boundingBox.x2;
            boxes[2].y2 = currentBox.y2;
            boxes[2].z2 = nodes[2]->boundingBox.z2;

            // creates the higher x, higher y, lower z octant box
            boxes[3].x1 = nodes[3]->boundingBox.x1;
            boxes[3].y1 = nodes[3]->boundingBox.y1;
            boxes[3].z1 = currentBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = nodes[3]->boundingBox.z2;

            // creates the lower x, lower y, higher z octant box
            boxes[4].x1 = currentBox.x1;
            boxes[4].y1 = currentBox.y1;
            boxes[4].z1 = nodes[4]->boundingBox.z1;
            boxes[4].x2 = nodes[4]->boundingBox.x2;
            boxes[4].y2 = nodes[4]->boundingBox.y2;
            boxes[4].z2 = currentBox.z2;

            // creates the higher x, lower y, higher z octant box
            boxes[5].x1 = nodes[5]->boundingBox.x1;
            boxes[5].y1 = currentBox.y1;
            boxes[5].z1 = nodes[5]->boundingBox.z1;
            boxes[5].x2 = currentBox.x2;
            boxes[5].y2 = nodes[5]->boundingBox.y2;
            boxes[5].z2 = currentBox.z2;

            // creates the lower x, higher y, higher z octant box
            boxes[6].x1 = currentBox.x1;
            boxes[6].y1 = nodes[6]->boundingBox.y1;
            boxes[6].z1 = nodes[6]->boundingBox.z1;
            boxes[6].x2 = nodes[6]->boundingBox.x2;
            boxes[6].y2 = currentBox.y2;
            boxes[6].z2 = currentBox.z2;

            // creates the higher x, higher y, higher z octant box
            boxes[7].x1 = nodes[7]->boundingBox.x1;
            boxes[7].y1 = nodes[7]->boundingBox.y1;
            boxes[7].z1 = nodes[7]->boundingBox.z1;
            boxes[7].x2 = currentBox.x2;
            boxes[7].y2 = currentBox.y2;
            boxes[7].z2 = currentBox.z2;

            // wraps and pushes the eight nodes for processing, along with respective boxes
            this->pushNodeBoxes(8, nodes, boxes, nodeBoxes, nodesStack);
        }
    }
}

/**
 * Retrieves the elements bounded by the provided query box.
 *
 * @param queryBox The box for which contained elements are retrieved.
 * @return The elements bounded by the provided box.
 */
std::vector<void *> OctTreeNode::getBoxElements(Box3d_t *queryBox) {
    std::map<void *, bool> elementsMap;
    std::vector<void *> elements;

    // initializes the stack of nodes to process
    std::vector<OctTreeNodeBox3d_t> nodesStack;

    // creates the variables for the current values
    OctTreeNodeBox3d_t currentNodeBox;
    OctTreeNode *currentNode;
    Box3d_t currentBox;
    void *currentElement;

    // declares the oct tree node boxes list
    OctTreeNodeBox3d_t nodeBoxes[8];

    // declares the list of oct tree node pointers
    OctTreeNode *nodes[8];

    // declares the list of bounding boxes
    Box3d_t boxes[8];

    // creates a node box with the start element
    nodeBoxes[0].node = this;
    nodeBoxes[0].box = *queryBox;

    // pushes the first oct tree node onto the stack
    nodesStack.push_back(nodeBoxes[0]);

    // until the stack is not empty
    while(!nodesStack.empty()) {
        // retrieves the current node box structure
        currentNodeBox = nodesStack.back();

        // removes the current node from the stack
        nodesStack.pop_back();

        // retrieves the current node
        currentNode = currentNodeBox.node;

        // retrieves the box with which to bound
        currentBox = currentNodeBox.box;

        // retrieves the number of elements in the current node
        unsigned int currentNodeElementsSize = currentNode->elements.size();

        // adds the current node's elements
        for(unsigned int i = 0; i < currentNodeElementsSize; i++) {
            currentElement = currentNode->elements[i];

            // in case the element is not in the bit map
            if(elementsMap[currentElement] != true) {
                // adds the current element
                elements.push_back(currentElement);

                // signals the element's bit in the map
                elementsMap[currentElement] = true;
            }
        }

        // tests for overlapping
        if (BoxUtil::overlaps(currentNode->boundingBox, currentBox)) {
            // continues the loop
            continue;
        }

        // in case the octant nodes have not been yet generated
        if (currentNode->childNodes.size() == 0) {
            // continues the loop
            continue;
        }

        // retrieves the query's bounding box's vertexes
        Coordinate3d_t leftTopBackPoint = {currentBox.x1, currentBox.y2, currentBox.z1};
        Coordinate3d_t rightTopBackPoint = {currentBox.x2, currentBox.y2, currentBox.z1};
        Coordinate3d_t leftBottomBackPoint = {currentBox.x1, currentBox.y1, currentBox.z1};
        Coordinate3d_t rightBottomBackPoint = {currentBox.x2, currentBox.y1, currentBox.z1};
        Coordinate3d_t leftTopFrontPoint = {currentBox.x1, currentBox.y2, currentBox.z2};
        Coordinate3d_t rightTopFrontPoint = {currentBox.x2, currentBox.y2, currentBox.z2};
        Coordinate3d_t leftBottomFrontPoint = {currentBox.x1, currentBox.y1, currentBox.z2};
        Coordinate3d_t rightBottomFrontPoint = {currentBox.x2, currentBox.y1, currentBox.z2};

        // retrieves the octants for each of the query's bounding box's vertexes
        int leftTopBackPointOctant = currentNode->getPointOctant(leftTopBackPoint);
        int rightTopBackPointOctant = currentNode->getPointOctant(rightTopBackPoint);
        int leftBottomBackPointOctant = currentNode->getPointOctant(leftBottomBackPoint);
        int rightBottomBackPointOctant = currentNode->getPointOctant(rightBottomBackPoint);
        int leftTopFrontPointOctant = currentNode->getPointOctant(leftTopFrontPoint);
        int rightTopFrontPointOctant = currentNode->getPointOctant(rightTopFrontPoint);
        int leftBottomFrontPointOctant = currentNode->getPointOctant(leftBottomFrontPoint);
        int rightBottomFrontPointOctant = currentNode->getPointOctant(rightBottomFrontPoint);

        // in case the query is contained by a single octant
        if(leftTopBackPointOctant == rightTopBackPointOctant &&
           leftTopBackPointOctant == leftBottomBackPointOctant &&
           leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the octant box
            boxes[0] = currentBox;

            // wraps and pushes the node for processing, along with respective box
            this->pushNodeBoxes(1, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element horizontally spans two octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the lower x octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x octant
            nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

            // creates the lower x octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = currentBox.y2;
            boxes[0].z2 = currentBox.z2;

            // creates the higher x octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = currentBox.z2;

            // wraps and pushes the two nodes for processing, along with respective boxes
            this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element vertically spans two octants
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the lower y octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher y octant
            nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

            // creates the lower y octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = currentBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = currentBox.z2;

            // creates the higher y octant box
            boxes[1].x1 = currentBox.x1;
            boxes[1].y1 = nodes[1]->boundingBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = currentBox.z2;

            // wraps and pushes the two nodes for processing, along with respective boxes
            this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans two octants accross the z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower z octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher z octant
            nodes[1] = currentNode->childNodes[leftTopFrontPointOctant];

            // creates the lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = currentBox.x2;
            boxes[0].y2 = currentBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher z octant box
            boxes[1].x1 = currentBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = nodes[1]->boundingBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = currentBox.z2;

            // wraps and pushes the two nodes for processing, along with respective boxes
            this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans four octants accross the x and y-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
            // determines the lower x, lower y octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher x, lower y octant
            nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

            // determines the lower x, higher y octant
            nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x, higher y octant
            nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

            // creates the lower x, lower y octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = currentBox.z2;

            // creates the higher x, lower y octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = nodes[1]->boundingBox.y2;
            boxes[1].z2 = currentBox.z2;

            // creates the lower x, higher y octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = nodes[2]->boundingBox.y1;
            boxes[2].z1 = currentBox.z1;
            boxes[2].x2 = nodes[2]->boundingBox.x2;
            boxes[2].y2 = currentBox.y2;
            boxes[2].z2 = currentBox.z2;

            // creates the higher x, higher y octant box
            boxes[3].x1 = nodes[3]->boundingBox.x1;
            boxes[3].y1 = nodes[3]->boundingBox.y1;
            boxes[3].z1 = currentBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = currentBox.z2;

            // wraps and pushes the four nodes for processing, along with respective boxes
            this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans four octants accross the x and z-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower x, lower z octant
            nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x, lower z octant
            nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

            // determines the lower x, higher z octant
            nodes[2] = currentNode->childNodes[leftTopFrontPointOctant];

            // determines the higher x, higher z octant
            nodes[3] = currentNode->childNodes[rightTopFrontPointOctant];

            // creates the lower x, lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = currentBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher x, lower z octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = nodes[1]->boundingBox.z2;

            // creates the lower x, higher z octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = currentBox.y1;
            boxes[2].z1 = nodes[2]->boundingBox.z1;
            boxes[2].x2 = nodes[2]->boundingBox.x2;
            boxes[2].y2 = currentBox.y2;
            boxes[2].z2 = currentBox.z2;

            // creates the higher x, higher z octant box
            boxes[3].x1 = nodes[3]->boundingBox.x1;
            boxes[3].y1 = currentBox.y1;
            boxes[3].z1 = nodes[3]->boundingBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = currentBox.z2;

            // wraps and pushes the four nodes for processing, along with respective boxes
            this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans four octants accross the y and z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower y, lower z octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher y, lower z octant
            nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the lower y, higher z octant
            nodes[2] = currentNode->childNodes[leftBottomFrontPointOctant];

            // determines the higher y, higher z octant
            nodes[3] = currentNode->childNodes[leftTopFrontPointOctant];

            // creates the lower y, lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = currentBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher y, lower z octant box
            boxes[1].x1 = currentBox.x1;
            boxes[1].y1 = nodes[1]->boundingBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = currentBox.y2;
            boxes[1].z2 = nodes[1]->boundingBox.z2;

            // creates the lower y, higher z octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = currentBox.y1;
            boxes[2].z1 = nodes[2]->boundingBox.z1;
            boxes[2].x2 = currentBox.x2;
            boxes[2].y2 = nodes[2]->boundingBox.y2;
            boxes[2].z2 = currentBox.z2;

            // creates the higher x, higher z octant box
            boxes[3].x1 = currentBox.x1;
            boxes[3].y1 = nodes[3]->boundingBox.y1;
            boxes[3].z1 = nodes[3]->boundingBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = currentBox.z2;

            // wraps and pushes the four nodes for processing, along with respective boxes
            this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the element spans eight octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
            // determines the lower x, lower y, lower z octant
            nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

            // determines the higher x, lower y, lower z octant
            nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

            // determines the lower x, higher y, lower z octant
            nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

            // determines the higher x, higher y, lower z octant
            nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

            // determines the lower x, lower y, higher z octant
            nodes[4] = currentNode->childNodes[leftBottomFrontPointOctant];

            // determines the higher x, lower y, higher z octant
            nodes[5] = currentNode->childNodes[rightBottomFrontPointOctant];

            // determines the lower x, higher y, higher z octant
            nodes[6] = currentNode->childNodes[leftTopFrontPointOctant];

            // determines the higher x, higher y, higher z octant
            nodes[7] = currentNode->childNodes[rightTopFrontPointOctant];

            // creates the lower x, lower y, lower z octant box
            boxes[0].x1 = currentBox.x1;
            boxes[0].y1 = currentBox.y1;
            boxes[0].z1 = currentBox.z1;
            boxes[0].x2 = nodes[0]->boundingBox.x2;
            boxes[0].y2 = nodes[0]->boundingBox.y2;
            boxes[0].z2 = nodes[0]->boundingBox.z2;

            // creates the higher x, lower y, lower z octant box
            boxes[1].x1 = nodes[1]->boundingBox.x1;
            boxes[1].y1 = currentBox.y1;
            boxes[1].z1 = currentBox.z1;
            boxes[1].x2 = currentBox.x2;
            boxes[1].y2 = nodes[1]->boundingBox.y2;
            boxes[1].z2 = nodes[1]->boundingBox.z2;

            // creates the lower x, higher y, lower z octant box
            boxes[2].x1 = currentBox.x1;
            boxes[2].y1 = nodes[2]->boundingBox.y1;
            boxes[2].z1 = currentBox.z1;
            boxes[2].x2 = nodes[2]->boundingBox.x2;
            boxes[2].y2 = currentBox.y2;
            boxes[2].z2 = nodes[2]->boundingBox.z2;

            // creates the higher x, higher y, lower z octant box
            boxes[3].x1 = nodes[3]->boundingBox.x1;
            boxes[3].y1 = nodes[3]->boundingBox.y1;
            boxes[3].z1 = currentBox.z1;
            boxes[3].x2 = currentBox.x2;
            boxes[3].y2 = currentBox.y2;
            boxes[3].z2 = nodes[3]->boundingBox.z2;

            // creates the lower x, lower y, higher z octant box
            boxes[4].x1 = currentBox.x1;
            boxes[4].y1 = currentBox.y1;
            boxes[4].z1 = nodes[4]->boundingBox.z1;
            boxes[4].x2 = nodes[4]->boundingBox.x2;
            boxes[4].y2 = nodes[4]->boundingBox.y2;
            boxes[4].z2 = currentBox.z2;

            // creates the higher x, lower y, higher z octant box
            boxes[5].x1 = nodes[5]->boundingBox.x1;
            boxes[5].y1 = currentBox.y1;
            boxes[5].z1 = nodes[5]->boundingBox.z1;
            boxes[5].x2 = currentBox.x2;
            boxes[5].y2 = nodes[5]->boundingBox.y2;
            boxes[5].z2 = currentBox.z2;

            // creates the lower x, higher y, higher z octant box
            boxes[6].x1 = currentBox.x1;
            boxes[6].y1 = nodes[6]->boundingBox.y1;
            boxes[6].z1 = nodes[6]->boundingBox.z1;
            boxes[6].x2 = nodes[6]->boundingBox.x2;
            boxes[6].y2 = currentBox.y2;
            boxes[6].z2 = currentBox.z2;

            // creates the higher x, higher y, higher z octant box
            boxes[7].x1 = nodes[7]->boundingBox.x1;
            boxes[7].y1 = nodes[7]->boundingBox.y1;
            boxes[7].z1 = nodes[7]->boundingBox.z1;
            boxes[7].x2 = currentBox.x2;
            boxes[7].y2 = currentBox.y2;
            boxes[7].z2 = currentBox.z2;

            // wraps and pushes the eight nodes for processing, along with respective boxes
            this->pushNodeBoxes(8, nodes, boxes, nodeBoxes, nodesStack);
        }
    }

    return elements;
}

/**
 * Allocates and returns a new child node for the provided bounding box.
 *
 * @param boundingBox The bounding box for the child node.
 */
inline OctTreeNode *OctTreeNode::createChildNode(Box3d_t &boundingBox) {
    return new OctTreeNode(boundingBox);
}

/**
 * Initializes the octant nodes by building eight sub-partitions for the node's partitioned space.
 */
inline void OctTreeNode::generateChildNodes() {
    float x1 = this->boundingBox.x1;
    float y1 = this->boundingBox.y1;
    float z1 = this->boundingBox.z1;

    float x2 = this->boundingBox.x2;
    float y2 = this->boundingBox.y2;
    float z2 = this->boundingBox.z2;

    float width = x2 - x1;
    float height = y2 - y1;
    float depth = z2 - z1;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;

    Box3d_t firstOctantBox = BoxUtil::createBox(x1, y1, z1, x1 + halfWidth, y1 + halfHeight, z1 + halfDepth);
    Box3d_t secondOctantBox = BoxUtil::createBox(x1 + halfWidth, y1, z1, x2, y1 + halfHeight, z1 + halfDepth);
    Box3d_t thirdOctantBox = BoxUtil::createBox(x1, y1 + halfHeight, z1, x1 + halfWidth, y2, z1 + halfDepth);
    Box3d_t fourthOctantBox = BoxUtil::createBox(x1 + halfWidth, y1 + halfHeight, z1, x2, y2, z1 + halfDepth);
    Box3d_t fifthOctantBox = BoxUtil::createBox(x1, y1, z1 + halfDepth, x1 + halfWidth, y1 + halfHeight, z2);
    Box3d_t sixthOctantBox = BoxUtil::createBox(x1 + halfWidth, y1, z1 + halfDepth, x2, y1 + halfHeight, z2);
    Box3d_t seventhOctantBox = BoxUtil::createBox(x1, y1 + halfHeight, z1 + halfDepth, x1 + halfWidth, y2, z2);
    Box3d_t eighthOctantBox = BoxUtil::createBox(x1 + halfWidth, y1 + halfHeight, z1 + halfDepth, x2, y2, z2);

    // creates the octant nodes
    OctTreeNode *firstOctantNode = this->createChildNode(firstOctantBox);
    OctTreeNode *secondOctantNode = this->createChildNode(secondOctantBox);
    OctTreeNode *thirdOctantNode = this->createChildNode(thirdOctantBox);
    OctTreeNode *fourthOctantNode = this->createChildNode(fourthOctantBox);
    OctTreeNode *fifthOctantNode = this->createChildNode(fifthOctantBox);
    OctTreeNode *sixthOctantNode = this->createChildNode(sixthOctantBox);
    OctTreeNode *seventhOctantNode = this->createChildNode(seventhOctantBox);
    OctTreeNode *eighthOctantNode = this->createChildNode(eighthOctantBox);

    // adds the octant nodes to the list
    this->childNodes.push_back(firstOctantNode);
    this->childNodes.push_back(secondOctantNode);
    this->childNodes.push_back(thirdOctantNode);
    this->childNodes.push_back(fourthOctantNode);
    this->childNodes.push_back(fifthOctantNode);
    this->childNodes.push_back(sixthOctantNode);
    this->childNodes.push_back(seventhOctantNode);
    this->childNodes.push_back(eighthOctantNode);
}

/**
 * Determines in which octant is the given point contained.
 *
 * @param point The point to determine the octant.
 * @return The octant node index which contains the specified point.
 */
inline int OctTreeNode::getPointOctant(const Coordinate3d_t &point) {
    int octantIndex = -1;

    // caches the childs nodes list size
    int childNodesSize = this->childNodes.size();

    // for each node
    for (int i = 0; i < childNodesSize; i++) {
        // retrieves the node
        OctTreeNode *childNode = this->childNodes[i];

        // in case the given point is inside the node
        if (BoxUtil::containsPoint(this->boundingBox, point)) {
            // signals the index was found
            octantIndex = i;
            break;
        }
    }

    // returns the retrieved index
    return octantIndex;
}

/**
 * Wraps an arbitrary number of nodes and their boxes into the composite stack element.
 */
inline void OctTreeNode::pushNodeBoxes(int node_count, OctTreeNode **nodes, Box3d_t *boxes, OctTreeNodeBox3d_t *nodeBoxes, std::vector<OctTreeNodeBox3d_t> &nodesStack) {
    for(int i = 0; i < node_count; i++) {
        // wraps the node and box
        nodeBoxes[i].node = nodes[i];
        nodeBoxes[i].box = boxes[i];

        // pushes the node box for processing
        nodesStack.push_back(nodeBoxes[i]);
    }
}

/**
 * Indicates if the oct tree node is big enough to be sub-divided.
 */
inline bool OctTreeNode::isSubdividable() {
    // retrieves the node's bounding box's extends
    float width = this->boundingBox.x2 - this->boundingBox.x1;
    float height = this->boundingBox.y2 - this->boundingBox.y1;
    float depth = this->boundingBox.z2 - this->boundingBox.z1;

    // computes the current bounded volume
    float volume = width * height * depth;

    // the node is subdividable if its volume is greater or equal to the minimum volume
    return volume >= MIN_SUBDIVIDABLE_VOLUME;
}
