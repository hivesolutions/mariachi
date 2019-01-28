// Hive Mariachi Engine
// Copyright (c) 2008-2019 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2019 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../nodes/square_node.h"

namespace mariachi {
    namespace ui {
        typedef enum UiNodesType_t {
            UI_NODE_TYPE = 2000,
            UI_COMPONENT_NODE_TYPE,
            UI_BOX_COMPONENT_NODE_TYPE,
            UI_VIEW_PORT_NODE_TYPE,
            UI_CONTAINER_NODE_TYPE,
            UI_PANEL_NODE_TYPE,
            UI_BUTTON_NODE_TYPE
        } UiNodesType;

        class UiNode : public nodes::SquareNode {
            private:

            public:
                UiNode();
                UiNode(const std::string &name);
                ~UiNode();
                virtual inline unsigned int getNodeType() { return UI_NODE_TYPE; };
        };
    }
}
