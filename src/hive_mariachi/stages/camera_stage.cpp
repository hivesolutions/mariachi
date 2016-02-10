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

#include "../camera/camera.h"

#include "camera_stage.h"

using namespace mariachi;
using namespace mariachi::stages;

/**
 * Constructor of the class.
 */
CameraStage::CameraStage() : Stage() {
}

/**
 * Constructor of the class.
 *
 * @param engine The currently used engine.
 */
CameraStage::CameraStage(Engine *engine) : Stage(engine) {
}

/**
 * Constructor of the class.
 *
 * @param engine The currently used engine.
 * @param name The name of the stage.
 */
CameraStage::CameraStage(Engine *engine, const std::string &name) : Stage(engine, name) {
}

/**
 * Destructor of the class.
 */
CameraStage::~CameraStage() {
}

void CameraStage::start(void *arguments) {
    Stage::start(arguments);

    // retrieves the camera manager
    this->cameraManager = this->engine->getCameraManager();
}

void CameraStage::stop(void *arguments) {
    Stage::stop(arguments);

    // unsets the camera manager
    this->cameraManager = NULL;
}

void CameraStage::update(void *arguments) {
    Stage::update(arguments);

    // updates the camera manager
    this->cameraManager->update(NULL);
}
