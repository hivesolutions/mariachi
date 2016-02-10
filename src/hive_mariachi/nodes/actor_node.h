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

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../structures/structures.h"

#include "model_node.h"

namespace mariachi {
    namespace nodes {
        class ActorNode : public ModelNode {
            private:
                std::vector<structures::Frame_t *> *frameList;
                unsigned int currentFrame;

            public:
                ActorNode();
                ActorNode(const std::string &name);
                ~ActorNode();
                inline void initRenderable();
                inline void initCurrentFrame();
                inline structures::Frame_t *getCurrentFrame();
                inline void incrementFrame();
                std::vector<structures::Mesh_t *> *getMeshList();
                std::vector<structures::Frame_t *> *getFrameList();
                void setFrameList(std::vector<structures::Frame_t *> *frameList);
                void updateFrame();
                void loop(const std::string &animationName);
                virtual inline unsigned int getNodeType() { return ACTOR_NODE_TYPE; };
        };
    }
}
