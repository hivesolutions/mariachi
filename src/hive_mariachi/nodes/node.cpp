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

#include "node.h"

using namespace mariachi::nodes;

/**
 * Constructor of the class.
 */
Node::Node() {
    this->initChildrenListMutex();
    this->initRenderable();
}

Node::Node(const std::string &name) {
    this->initChildrenListMutex();
    this->initRenderable();
    this->name = name;
}

/**
 * Destructor of the class.
 */
Node::~Node() {
    // closes the children list mutex
    MUTEX_CLOSE(this->childrenListMutexHandle);
}

inline void Node::initChildrenListMutex() {
    // creates the children list mutex
    MUTEX_CREATE(this->childrenListMutexHandle);
}

inline void Node::initRenderable() {
    this->renderable = false;
}

Node *Node::getParent() {
    return this->parent;
}

void Node::setParent(Node *parent) {
    this->parent = parent;
}

void Node::reparentTo(Node *parent) {
    parent->addChild(this);
}

/**
 * Adds a child node to the current node.
 *
 * @param child The child node to be added.
 */
void Node::addChild(Node *child) {
    // locks the children list mutex
    MUTEX_LOCK(childrenListMutexHandle);

    // adds the child node to the children list
    this->childrenList.push_back(child);

    // sets the child node parent
    child->setParent(this);

    // unlocks the children list mutex
    MUTEX_UNLOCK(childrenListMutexHandle);
}

/**
 * Adds a child node to the current node.
 * This operation is unsafe as it does not use the access
 * mutex to write the list. Use this method carrefully.
 *
 * @param child The child node to be added.
 */
void Node::_addChild(Node *child) {
    // adds the child node to the children list
    this->childrenList.push_back(child);

    // sets the child node parent
    child->setParent(this);
}

/**
 * Adds a vector of child nodes to the current node.
 *
 * @param _childrenList The list of children nodes to be added.
 */
void Node::addChildren(std::vector<Node *> &_childrenList) {
    // locks the children list mutex
    MUTEX_LOCK(childrenListMutexHandle);

    // retrieves the children list iterator
    std::vector<Node *>::iterator childrenListIterator = _childrenList.begin();

    // iterates over all the child nodes in the children list
    while(childrenListIterator != _childrenList.end()) {
        // retrieves the current child node
        Node *child = *childrenListIterator;

        // adds the child node to the children list
        this->childrenList.push_back(child);

        // sets the child node parent
        child->setParent(this);

        // increments the children list iterator
        childrenListIterator++;
    }

    // unlocks the children list mutex
    MUTEX_UNLOCK(childrenListMutexHandle);
}

/**
 * Removes a child node from the current node.
 * This operation is unsafe as it does not use the access
 * mutex to write the list. Use this method carrefully.
 *
 * @param child The child node to be removed.
 */
void Node::removeChild(Node *child) {
    // locks the children list mutex
    MUTEX_LOCK(childrenListMutexHandle);

    // removes the child node from the children list
    this->childrenList.remove(child);

    // resets the child node parent
    child->setParent(NULL);

    // unlocks the children list mutex
    MUTEX_UNLOCK(childrenListMutexHandle);
}

/**
 * Removes a child node from the current node.
 *
 * @param child The child node to be removed.
 */
void Node::_removeChild(Node *child) {
    // removes the child node from the children list
    this->childrenList.remove(child);

    // resets the child node parent
    child->setParent(NULL);
}

/**
 * Removes a vector of child nodes from the current node.
 *
 * @param _childrenList The list of children nodes to be removed.
 */
void Node::removeChildren(std::vector<Node *> &_childrenList) {
    // locks the children list mutex
    MUTEX_LOCK(childrenListMutexHandle);

    // retrieves the children list iterator
    std::vector<Node *>::iterator childrenListIterator = _childrenList.begin();

    // iterates over all the child nodes in the children list
    while(childrenListIterator != _childrenList.end()) {
        // retrieves the current child node
        Node *child = *childrenListIterator;

        // removes the child node from the children list
        this->childrenList.remove(child);

        // resets the child node parent
        child->setParent(NULL);

        // increments the children list iterator
        childrenListIterator++;
    }

    // unlocks the children list mutex
    MUTEX_UNLOCK(childrenListMutexHandle);
}

bool Node::isRenderable() {
    return this->renderable;
}

std::list<Node *> &Node::getChildrenList() {
    return this->childrenList;
}

void Node::setChildrenList(std::list<Node *> &childrenList) {
    this->childrenList = childrenList;
}

bool Node::getRenderable() {
    return this->renderable;
}

void Node::setRenderable(bool renderable) {
    this->renderable = renderable;
}

Node *Node::duplicate() {
    // creates the new reference node
    Node *referenceNode = new Node();

    // returns the reference node
    return referenceNode;
}
