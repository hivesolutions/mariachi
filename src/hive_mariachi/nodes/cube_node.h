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

#include "../structures/position.h"
#include "../structures/rotation.h"

#include "node.h"

namespace mariachi {
    namespace nodes {
        class CubeNode : public Node {
            private:
                structures::Coordinate3d_t position;
                structures::Rotation3d_t rotation;
                structures::Coordinate3d_t scale;

            public:
                CubeNode();
                CubeNode(const std::string &name);
                ~CubeNode();
                inline void initTransforms();
                structures::Coordinate3d_t &getPosition();
                void setPosition(structures::Coordinate3d_t &position);
                void setPosition(float x, float y, float z);
                structures::Rotation3d_t &getRotation();
                void setRotation(structures::Rotation3d_t &rotation);
                void setRotation(float angle, float x, float y, float z);
                void setRotation(float xAxisDegrees, float yAxisDegrees, float zAxisDegrees);
                structures::Coordinate3d_t &getScale();
                void setScale(structures::Coordinate3d_t &scale);
                void setScale(float scaleX, float scaleY, float scaleZ);
                virtual inline unsigned int getNodeType() { return CUBE_NODE_TYPE; };
        };
    }
}
