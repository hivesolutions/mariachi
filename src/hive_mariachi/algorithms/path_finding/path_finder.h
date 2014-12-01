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

#pragma once

#include "../../structures/path.h"

namespace mariachi {
    namespace algorithms {
        class PathFinder {
            private:
                float (*costFunction)(unsigned int, unsigned int, structures::PathNodesGraph *);

            protected:
                structures::PathNodesGraph *nodesGraph;

            public:
                PathFinder();
                PathFinder(structures::PathNodesGraph *nodesGraph);
                ~PathFinder();
                inline void initNodesGraph(structures::PathNodesGraph *nodesGraph);
                inline void initCostFunction();
                virtual structures::Path_t *findPath(unsigned int startNodeId, unsigned int endNodeId) { return NULL; };
                virtual structures::PathNodesGraph *getNodesGraph() { return this->nodesGraph; };
                virtual void setNodesGraph(structures::PathNodesGraph *nodesGraph) { this->nodesGraph = nodesGraph; };
                virtual void setCostFunction(float (*costFunction)(unsigned int, unsigned int, structures::PathNodesGraph *)) { this->costFunction = costFunction; };
                virtual float (*getCostFunction())(unsigned int, unsigned int, structures::PathNodesGraph *) { return this->costFunction; };
                static float defaultCostFunction(unsigned int firstNodeId, unsigned int secondNodeId, structures::PathNodesGraph *nodesGraph);
        };
    }
}
