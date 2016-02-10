// Hive Mariachi Engine
// Copyright (c) 2008-2016 Hive Solutions Lda.
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

// __author__    = Jo�o Magalh�es <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "path_finder.h"

namespace mariachi {
    namespace algorithms {
        class AStar : public PathFinder {
            private:
                float (*distanceFunction)(unsigned int, unsigned int, structures::PathNodesGraph *);

            public:
                AStar();
                AStar(structures::PathNodesGraph *nodesGraph);
                ~AStar();
                inline void initDistanceFunction();
                structures::Path_t *findPath(unsigned int startNodeId, unsigned int endNodeId);
                void setDistanceFunction(float (*distanceFunction)(unsigned int, unsigned int, structures::PathNodesGraph *)) { this->distanceFunction = distanceFunction; };
                float (*getDistanceFunction())(unsigned int, unsigned int, structures::PathNodesGraph *) { return this->distanceFunction; };
                bool operator()(std::pair<unsigned int, float> &firstPair, std::pair<unsigned int, float> &secondPair);
                static float defaultDistanceFunction(unsigned int firstNodeId, unsigned int secondNodeId, structures::PathNodesGraph *nodesGraph);
        };
    }
}
