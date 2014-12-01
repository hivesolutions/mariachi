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

#include "devices/devices.h"

#include "debug_engine.h"

using namespace mariachi::debugging;

/**
 * Constructor of the class.
 */
DebugEngine::DebugEngine() {
}

/**
 * Destructor of the class.
 */
DebugEngine::~DebugEngine() {
}

void DebugEngine::start(void *arguments) {
    // starts the debug devices
    this->startDebugDevices();
}

void DebugEngine::stop(void *arguments) {
}

/**
 * Starts the debug devices in the debug engine.
 * Starting the debug devices implies starting the drivers and start listning
 * to them.
 */
void DebugEngine::startDebugDevices() {
    // creates a new network keyboard
    NetworkKeyboard *networkKeyboard = new NetworkKeyboard();

    // starts the network keyboard
    networkKeyboard->start(NULL);

    // sets the network keyboard in the debug devices registry
    this->setDebugDevice("network_keyboard", networkKeyboard);
}

/**
 * Retrieves the debug device for the given debug device name.
 *
 * @param debugDeviceName The debug device name to retrieve the debug device.
 * @return The debug device for the given debug device name.
 */
DebugDevice *DebugEngine::getDebugDevice(const std::string &debugDeviceName) {
    return this->debugDevicesMap[debugDeviceName];
}

/**
 * Sets the debug device with the given debug device name.
 *
 * @param debugDeviceName The name to be used to identify the debug device.
 * @param debugDevice The debug device to be set.
 */
void DebugEngine::setDebugDevice(const std::string &debugDeviceName, DebugDevice *debugDevice) {
    // sets the debug device in the debug devices map with
    // the given debug device name
    this->debugDevicesMap[debugDeviceName] = debugDevice;
}
