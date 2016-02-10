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
// __copyright__ = Copyright (c) 2008-2016 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../structures/size.h"
#include "../structures/color.h"
#include "../structures/texture.h"
#include "ui_node.h"

namespace mariachi {
    namespace ui {
        class ComponentNode : public UiNode {
            private:
                structures::FloatSize2d_t size;
                structures::FloatColor_t color;
                structures::Texture *texture;

            public:
                ComponentNode();
                ComponentNode(const std::string &name);
                ~ComponentNode();
                void initRenderable();
                structures::FloatColor_t &getColor();
                void setColor(const structures::FloatColor_t &color);
                structures::Texture *getTexture();
                void setTexture(structures::Texture *texture);
                virtual inline unsigned int getNodeType() { return UI_COMPONENT_NODE_TYPE; };
        };
    }
}
