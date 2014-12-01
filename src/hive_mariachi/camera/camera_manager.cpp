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

#include "stdafx.h"

#include "camera_manager.h"

using namespace mariachi;
using namespace mariachi::nodes;
using namespace mariachi::camera;
using namespace mariachi::structures;

/**
 * Constructor of the class.
 */
CameraManager::CameraManager() {
    this->initDefaultCamera();
    this->initTransitionCamera();
}

CameraManager::CameraManager(Engine *engine) {
    this->initDefaultCamera();
    this->initTransitionCamera();
    this->initEngine(engine);
}

/**
 * Destructor of the class.
 */
CameraManager::~CameraManager() {
    // destroys the default camera
    delete this->defaultCamera;

    // destroy the active camera
    delete this->activeCamera;
}

/**
 * Initializes the default camera.
 */
inline void CameraManager::initDefaultCamera() {
    // creates the default camera
    this->defaultCamera = new CameraNode(std::string("default_camera"));

    // sets the default camera as the current active camera
    this->activeCamera = this->defaultCamera;
}

/**
 * Initializes the transition camera.
 */
inline void CameraManager::initTransitionCamera() {
    // creates the transition camera
    this->transitionCamera = new CameraNode(std::string("transition_camera"));

    // resets the start camera
    this->transitionStartCamera = NULL;

    // resets the end camera
    this->transitionEndCamera = NULL;

    // resets the transition duration
    this->transitionDuration = NULL;

    // resets the elapsed transition steps
    this->transitionElapsedSteps = NULL;
}

inline void CameraManager::initEngine(Engine *engine) {
    this->engine = engine;
}

void CameraManager::load(void *arguments) {
}

void CameraManager::unload(void *arguments) {
}

void CameraManager::update(void *arguments) {
    this->activeCamera->_autoFollow();
    this->activeCamera->_autoTrack();

    // in case a transition is in progress
    if(this->isTransitioning()) {
        // updates the transition
        this->updateTransition(1);
    }
}

/**
 * Retrieves the default camera.
 *
 * @return The default camera.
 */
CameraNode *CameraManager::getDefaultCamera() {
    return this->defaultCamera;
}

/**
 * Sets the default camera.
 *
 * @param defaultCamera The default camera.
 */
void CameraManager::setDefaultCamera(CameraNode *defaultCamera) {
    this->defaultCamera = defaultCamera;
}

/**
 * Retrieves the current active camera.
 *
 * @return The current active camera.
 */
CameraNode *CameraManager::getActiveCamera() {
    return this->activeCamera;
}

/**
 * Sets the current active camera.
 *
 * @param activeCamera The current active camera.
 */
void CameraManager::setActiveCamera(CameraNode *activeCamera) {
    this->activeCamera = activeCamera;
}

/**
 * Retrieves the camera for the given camera name.
 *
 * @param cameraName The camera name to retrieve the camera.
 * @return The camera for the given camera name.
 */
CameraNode *CameraManager::getCamera(const std::string &cameraName) {
    return this->camerasMap[cameraName];
}

/**
 * Sets the camera with the given camera name.
 *
 * @param cameraName The name to be used to identify the camera.
 * @param camera The camera to be set.
 */
void CameraManager::setCamera(const std::string &cameraName, CameraNode *camera) {
    this->camerasMap[cameraName] = camera;
}

/**
 * Sets up a transition between the current active camera and
 * the given camera.
 * @param cameraName The camera name to retrieve the camera to which
 * the transition is to be made.
 * @param duration The duration, in steps, of the transition animation.
 */
void CameraManager::startTransition(const std::string &cameraName, int duration) {
    // cancels existing transitions
    this->cancelTransition();

    // stores a reference to the start camera
    this->transitionStartCamera = this->activeCamera;

    // stores a reference to the end camera
    this->transitionEndCamera = this->getCamera(cameraName);

    // sets the transition camera as the current active camera
    this->activeCamera = this->transitionCamera;

    // starts the transition counter
    this->transitionDuration = duration;
}

/**
 * Updates the ongoing transition between cameras.
 */
void CameraManager::updateTransition(float elapsedSteps) {
    // updates the elapsed steps
    this->transitionElapsedSteps += elapsedSteps;

    // in case the transition is complete
    if(this->transitionElapsedSteps >= this->transitionDuration) {
        // sets the end camera as the active camera
        this->activeCamera = this->transitionEndCamera;

        // resets the start camera
        this->transitionStartCamera = NULL;

        // resets the end camera
        this->transitionEndCamera = NULL;

        // resets the transition duration
        this->transitionDuration = NULL;

        // resets the transition elapsed steps
        this->transitionElapsedSteps = NULL;

        // skips the transition update
        return;
    }

    // computes the transition progress
    float transitionProgress = this->transitionElapsedSteps / this->transitionDuration;

    // updates the start camera
    this->transitionStartCamera->_autoTrack();
    this->transitionStartCamera->_autoFollow();

    // updates the end camera
    this->transitionEndCamera->_autoTrack();
    this->transitionEndCamera->_autoFollow();

    // retrieves the start camera's position
    const Coordinate3d_t &transitionStartCameraPosition = this->transitionStartCamera->getPosition();

    // retrieves the start camera's rotation
    const Rotation3d_t &transitionStartCameraRotation = this->transitionStartCamera->getRotation();

    // retrieves the start camera's up vector
    const Coordinate3d_t &transitionStartCameraUpVector = this->transitionStartCamera->getUpVector();

    // retrieves the end camera's position
    const Coordinate3d_t &transitionEndCameraPosition = this->transitionEndCamera->getPosition();

    // retrieves the end camera's rotation
    const Rotation3d_t &transitionEndCameraRotation = this->transitionEndCamera->getRotation();

    // retrieves the end camera's up vector
    const Coordinate3d_t &transitionEndCameraUpVector = this->transitionEndCamera->getUpVector();

    // determines the transition camera's position by interpolation
    float transitionCameraPositionX = transitionStartCameraPosition.x + (transitionEndCameraPosition.x - transitionStartCameraPosition.x) * transitionProgress;
    float transitionCameraPositionY = transitionStartCameraPosition.y + (transitionEndCameraPosition.y - transitionStartCameraPosition.y) * transitionProgress;
    float transitionCameraPositionZ = transitionStartCameraPosition.z + (transitionEndCameraPosition.z - transitionStartCameraPosition.z) * transitionProgress;

    // determines the transition camera's rotation by interpolation
    float transitionCameraRotationAngle = transitionStartCameraRotation.angle + (transitionEndCameraRotation.angle - transitionStartCameraRotation.angle) * transitionProgress;
    float transitionCameraRotationX = transitionStartCameraRotation.x + (transitionEndCameraRotation.x - transitionStartCameraRotation.x) * transitionProgress;
    float transitionCameraRotationY = transitionStartCameraRotation.y + (transitionEndCameraRotation.y - transitionStartCameraRotation.y) * transitionProgress;
    float transitionCameraRotationZ = transitionStartCameraRotation.z + (transitionEndCameraRotation.z - transitionStartCameraRotation.z) * transitionProgress;

    // determines the transition camera's up vector by interpolation
    float transitionCameraUpVectorX = transitionStartCameraUpVector.x + (transitionEndCameraUpVector.x - transitionStartCameraUpVector.x) * transitionProgress;
    float transitionCameraUpVectorY = transitionStartCameraUpVector.y + (transitionEndCameraUpVector.y - transitionStartCameraUpVector.y) * transitionProgress;
    float transitionCameraUpVectorZ = transitionStartCameraUpVector.z + (transitionEndCameraUpVector.z - transitionStartCameraUpVector.z) * transitionProgress;

    // updates the transition camera's position
    this->transitionCamera->setPosition(transitionCameraPositionX, transitionCameraPositionY, transitionCameraPositionZ);

    // updates the transition camera's rotation
    this->transitionCamera->setRotation(transitionCameraRotationAngle, transitionCameraRotationX, transitionCameraRotationY, transitionCameraRotationZ);

    // updates the transition camera's up vector
    this->transitionCamera->setUpVector(transitionCameraUpVectorX, transitionCameraUpVectorY, transitionCameraUpVectorZ);
}

/**
 * Cancels the currently active transition.
 */
void CameraManager::cancelTransition() {
    // in case a transition is not in progress
    if(!this->isTransitioning()) {
        // skips the cancel
        return;
    }

    // sets the start camera as the active one
    this->activeCamera = this->transitionStartCamera;

    // resets the start camera
    this->transitionStartCamera = NULL;

    // resets the end camera
    this->transitionEndCamera = NULL;

    // resets the transition duration
    this->transitionDuration = NULL;

    // resets the transition elapsed steps
    this->transitionElapsedSteps = NULL;
}

/**
 * Indicates if the manager is currently transitioning
 * between cameras.
 */
bool CameraManager::isTransitioning() {
    // indicates if a valid transition duration exists
    return this->transitionDuration != NULL;
}
