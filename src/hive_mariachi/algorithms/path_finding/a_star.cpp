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

// __author__    = Tiago Silva <tsilva@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "a_star.h"

using namespace mariachi::algorithms;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
AStar::AStar() : PathFinder() {
    this->initDistanceFunction();
}

AStar::AStar(PathNodesGraph *nodesGraph) : PathFinder(nodesGraph) {
    this->initDistanceFunction();
}

/**
 * Destructor of the class.
 */
AStar::~AStar() {
}

inline void AStar::initDistanceFunction() {
    this->distanceFunction = AStar::defaultDistanceFunction;
}

Path_t *AStar::findPath(unsigned int startNodeId, unsigned int endNodeId) {
    // the unexplored nodes ordered by lowest cost
    std::priority_queue<std::pair<unsigned int, float>, std::vector<std::pair<unsigned int, float> >, AStar> unexploredNodes;

    // map indicating if a node was explored or not
    std::map<unsigned int, bool> exploredMap;

    // map associating a node with the lowest cost found to reach it
    std::map<unsigned int, float> costMap;

    // map associating a node with the sum of the lowest cost found to reach it plus the heuristic
    std::map<unsigned int, float> totalMap;

    // number nodes used to reach the node via the lowest cost path
    std::map<unsigned int, unsigned int> numberNodesMap;

    // map associating a node with the lowest cost node that was used to reach it
    std::map<unsigned int, unsigned int> previousNodeMap;

    // calculates the distance from the start to the end node
    float distance = this->distanceFunction(startNodeId, endNodeId, this->nodesGraph);

    // stores the cost for the start node
    costMap[startNodeId] = 0;
    totalMap[startNodeId] = distance;
    numberNodesMap[startNodeId] = 1;

    // adds the start node to the open set queue and marks it as unexplored
    unexploredNodes.push(std::pair<unsigned int, float>(startNodeId, distance));
    exploredMap[startNodeId] = false;

    unsigned int numberIterations = 0;

    // while the open set is not empty
    while(unexploredNodes.size() > 0) {
        // retrieves the node with lowest total cost
        std::pair<unsigned int, float> currentNodePair = unexploredNodes.top();
        unsigned int currentNodeId = currentNodePair.first;

        // returns the path in case the goal as been reached
        if(currentNodeId == endNodeId) {
            // creates a path structure where to store the resulting path
            unsigned int numberNodes = numberNodesMap[endNodeId];
            Path_t *path = (Path_t *) malloc(sizeof(Path_t));
            path->numberPathNodes = numberNodes;
            path->pathNodes = (unsigned int *) malloc(sizeof(unsigned int) * numberNodes);

            // backtracks from the end node storing the nodes in the path structure
            unsigned int index = 0;
            while(previousNodeMap.find(currentNodeId) != previousNodeMap.end()) {
                path->pathNodes[path->numberPathNodes - 1 - index] = currentNodeId;
                currentNodeId = previousNodeMap[currentNodeId];
                index++;
            }

            // stores the last node in the path nodes list
            path->pathNodes[path->numberPathNodes - 1 - index] = currentNodeId;

            return path;
        }

        // removes the node from the open set and marks it as explored
        unexploredNodes.pop();
        exploredMap[currentNodeId] = true;

        // iterates through all of the current node's neighbours
        PathNode_t *currentNode = (*this->nodesGraph)[currentNodeId];
        for(std::map<unsigned int, float>::const_iterator iterator = currentNode->neighboursMap->begin(); iterator != currentNode->neighboursMap->end(); iterator++) {
            // retrieves the neighbours' id and cost from the current node
            unsigned int neighbourNodeId = (*iterator).first;
            float movementCost = (*iterator).second;

            // skips in case the neighbour was already explored
            std::map<unsigned int, bool>::iterator exploredMapIterator = exploredMap.find(neighbourNodeId);
            bool neighbourFirstEncounter = exploredMapIterator == exploredMap.end();
            if(!neighbourFirstEncounter && exploredMapIterator->second) {
                continue;
            }

            // calculates the cost between the current node and the neighbour
            float newCost = costMap[currentNodeId];

            if(movementCost) {
                newCost += movementCost;
            } else {
                newCost += this->getCostFunction()(currentNodeId, neighbourNodeId, this->nodesGraph);
            }

            // calculates the distance from the neighbour node to the end node
            float distance = this->distanceFunction(neighbourNodeId, endNodeId, this->nodesGraph);
            float newTotal = newCost + distance;

            // skips the iteration in case the cost to travel to the neighbour is higher
            // than the previously found
            if(!neighbourFirstEncounter && newTotal > totalMap[neighbourNodeId]) {
                continue;
            }

            // marks the current node as the best path to the neighbour node
            previousNodeMap[neighbourNodeId] = currentNodeId;

            // stores the cost values for the best path to the neighbour node
            costMap[neighbourNodeId] = newCost;
            totalMap[neighbourNodeId] = newTotal;

            // stores the number of nodes traversed to reach this node
            numberNodesMap[neighbourNodeId] = numberNodesMap[currentNodeId] + 1;

            // adds the node to the open set in case it wasn't added before
            if(neighbourFirstEncounter) {
                unexploredNodes.push(std::pair<unsigned int, float>(neighbourNodeId, newTotal));
                exploredMap[neighbourNodeId] = false;
            }
        }

        numberIterations++;
    }

    return NULL;
}

bool AStar::operator()(std::pair<unsigned int, float> &firstPair, std::pair<unsigned int, float> &secondPair) {
    return firstPair.second > secondPair.second;
}

float AStar::defaultDistanceFunction(unsigned int firstNodeId, unsigned int secondNodeId, PathNodesGraph *nodesGraph) {
    return 0.0;
}
