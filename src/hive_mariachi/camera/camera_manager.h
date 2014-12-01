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

#include "../main/engine.h"
#include "../nodes/camera_node.h"

namespace mariachi {
    namespace camera {
        class CameraManager {
            private:
                /**
                 * The reference to the engine.
                 */
                Engine *engine;

                /**
                 * The default camera node.
                 */
                nodes::CameraNode *defaultCamera;

                /**
                 * The current active camera node.
                 */
                nodes::CameraNode *activeCamera;

                /**
                 * The map associating the camera name with the
                 * camera reference.
                 */
                std::map<std::string, nodes::CameraNode *> camerasMap;

                /**
                 * The intermediate camera used during the transition.
                 */
                nodes::CameraNode *transitionCamera;

                /**
                 * The camera with which the current transition started.
                 */
                nodes::CameraNode *transitionStartCamera;

                /**
                 * The camera with which the current transition will end.
                 */
                nodes::CameraNode *transitionEndCamera;

                /**
                 * The duration, in steps, for the current transition.
                 */
                float transitionDuration;

                /**
                 * The steps already elapsed in the current transition.
                 */
                float transitionElapsedSteps;

                inline void initEngine(Engine *engine);
                inline void initDefaultCamera();
                inline void initTransitionCamera();

            public:
                CameraManager();
                CameraManager(Engine *engine);
                ~CameraManager();
                void load(void *arguments);
                void unload(void *arguments);
                void update(void *arguments);
                nodes::CameraNode *getDefaultCamera();
                void setDefaultCamera(nodes::CameraNode *defaultCamera);
                nodes::CameraNode *getActiveCamera();
                void setActiveCamera(nodes::CameraNode *activeCamera);
                nodes::CameraNode *getCamera(const std::string &cameraName);
                void setCamera(const std::string &cameraName, nodes::CameraNode *camera);
                void startTransition(const std::string &cameraName, int duration);
                void updateTransition(float elapsedSteps);
                void cancelTransition();
                bool isTransitioning();
        };
    }
}
