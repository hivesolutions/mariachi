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

#include "oct_tree_node.h"
#include "box.h"

namespace mariachi {
    namespace structures {
        class OctTreeNode;

        /**
         * Represents an oct tree in 3d space.
         * Used for partioning a 3d environment into octants for faster spatial queries.
         */
        class OctTree {
            private:

            public:
                /**
                 * The tree's root node.
                 */
                OctTreeNode *rootNode;

                OctTree();
                OctTree(Box3d_t boundingBox);
                OctTree(float boxWidth, float boxHeight, float boxDepth);
                ~OctTree();
                void insertElementBox(void *element, Box3d_t *elementBoundingBox);
                std::vector<void *> getBoxElements(Box3d_t *queryBox);
        };
    }
}
