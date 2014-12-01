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
