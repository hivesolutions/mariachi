// Hive Mariachi Engine
// Copyright (c) 2008-2016 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2016 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "render_information.h"

using namespace mariachi::nodes;
using namespace mariachi::render;

/**
 * Constructor of the class.
 */
RenderInformation::RenderInformation() {
    // creates the mutex
    MUTEX_CREATE(this->mutex);
}

/**
 * Destructor of the class.
 */
RenderInformation::~RenderInformation() {
    // closes the mutex handle
    MUTEX_CLOSE(this->mutex);
}

SceneNode *RenderInformation::getRender() {
    return this->render;
}

void RenderInformation::setRender(SceneNode *render) {
    this->render = render;
}


Scene2dNode *RenderInformation::getRender2d() {
    return this->render2d;
}

void RenderInformation::setRender2d(Scene2dNode *render2d) {
    this->render2d = render2d;
}

CameraNode *RenderInformation::getActiveCamera() {
    return this->activeCamera;
}

void RenderInformation::setActiveCamera(CameraNode *activeCamera) {
    this->activeCamera = activeCamera;
}

MUTEX_HANDLE RenderInformation::getMutex() {
    return this->mutex;
}

void RenderInformation::setMutex(MUTEX_HANDLE mutex) {
    this->mutex = mutex;
}
