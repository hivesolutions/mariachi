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

#pragma once

#include "../nodes/nodes.h"

#include "box.h"
#include "position.h"

// the minimum volume of an octant
#define MIN_VOLUME 1.0f

// the minimum volume a subdividable octant can have
#define MIN_SUBDIVIDABLE_VOLUME MIN_VOLUME * 8.0f

namespace mariachi {
    namespace structures {
        struct OctTreeNodeBox3d_t;

        /**
         * Represents a node of an oct tree in 3d space.
         * Used to implement the underlying behaviour of the tree.
         */
        class OctTreeNode {
            private:
                /**
                 * The node's children.
                 */
                std::vector<OctTreeNode *> childNodes;

                /**
                 * The elements's contained by the space delimited by the node
                 */
                std::vector<void *> elements;

                /**
                 * The node's bounding box.
                 */
                Box3d_t boundingBox;

                inline OctTreeNode *createChildNode(Box3d_t &boundingBox);
                inline void generateChildNodes();
                inline int getPointOctant(const Coordinate3d_t &point);
                inline void pushNodeBoxes(int node_count, OctTreeNode **nodes, Box3d_t *boxes, OctTreeNodeBox3d_t *nodeBoxes, std::vector<OctTreeNodeBox3d_t> &nodesStack);
                inline bool isSubdividable();

            public:
                OctTreeNode();
                OctTreeNode(Box3d_t boundingBox);
                OctTreeNode(float boxWidth, float boxHeight, float boxDepth);
                ~OctTreeNode();
                void insertElementBox(void *element, Box3d_t *elementBoundingBox);
                std::vector<void *> getBoxElements(Box3d_t *queryBox);
                std::string toString(std::string padding);
        };

        /**
         * A structure for internal use containing an oct tree node, and a box.
         * Used as the elements for the stack based traversal of the oct tree.
         *
         * @param node The oct tree node at which the iteration is currently at.
         * @param box The box that is currently being processed.
         */
        typedef struct OctTreeNodeBox3d_t {
            OctTreeNode *node;
            Box3d_t box;
        } OctTreeNodeBox;
    }
}
