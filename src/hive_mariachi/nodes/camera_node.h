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

#include "lens_node.h"

namespace mariachi {
    namespace nodes {
        class CameraNode : public LensNode {
            private:
                CubeNode *autoTrackTargetNode;
                CubeNode *autoFollowTargetNode;
                structures::Coordinate3d_t autoTrackOffset;
                structures::Coordinate3d_t autoFollowOffset;
                structures::Coordinate3d_t upVector;

                inline void initializeAutoTracking();
                inline void initializeAutoFollowing();
                void setDirection(const structures::Coordinate3d_t &direction);
                void setDirection(float x, float y, float z);

            public:
                CameraNode();
                CameraNode(const std::string &name);
                ~CameraNode();
                void lookAt(const structures::Coordinate3d_t &targetPoint);
                void lookAt(float x, float y, float z);
                bool isAutoTracking();
                bool isAutoFollowing();
                void enableAutoTracking(CubeNode *targetNode);
                void enableAutoTracking(CubeNode *targetNode, structures::Coordinate3d_t &offset);
                void disableAutoTracking();
                void enableAutoFollowing(CubeNode *targetNode);
                void enableAutoFollowing(CubeNode *targetNode, structures::Coordinate3d_t &offset);
                void disableAutoFollowing();
                structures::Coordinate3d_t &getUpVector();
                void setUpVector(const structures::Coordinate3d_t &upVector);
                void setUpVector(float x, float y, float z);
                void _autoTrack();
                void _autoFollow();
                virtual inline unsigned int getNodeType() { return CAMERA_NODE_TYPE; };
        };
    }
}
