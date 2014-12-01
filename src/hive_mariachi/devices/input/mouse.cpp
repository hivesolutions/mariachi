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

#include "mouse.h"

using namespace mariachi::devices;

/**
 * Constructor of the class.
 */
Mouse::Mouse() : InputDevice() {
    // sets the mouse event buffer size as the default one
    this->mouseEventBufferSize = DEFAULT_MOUSE_EVENT_BUFFER_SIZE;

    // initializes the mouse event buffer
    this->initMouseEventBuffer();

    // initializes the mouse move capture
    this->initMouseMoveCapture();
}

Mouse::Mouse(unsigned int mouseEventBufferSize) {
    // sets the mouse event buffer size
    this->mouseEventBufferSize = mouseEventBufferSize;

    // initializes the mouse event buffer
    this->initMouseEventBuffer();

    // initializes the mouse move capture
    this->initMouseMoveCapture();
}

Mouse::~Mouse() {
    // releases the mouse event buffer
    free(this->mouseEventBuffer);
}

inline void Mouse::initMouseEventBuffer() {
    // allocates space for the mouse event buffer
    this->mouseEventBuffer = (MouseEvent_t *) malloc(sizeof(MouseEvent_t) * this->mouseEventBufferSize);

    // resets the mouse event buffer position
    this->mouseEventBufferPosition = 0;
}

inline void Mouse::initMouseMoveCapture() {
    this->mouseMoveCapture = DEFAULT_MOUSE_MOVE_CAPTURE;
}

inline MouseEvent_t *Mouse::getMouseEvent() {
    // in case the mouse event buffer position is greater or equal than
    // the mouse event buffer size
    if(this->mouseEventBufferPosition >= this->mouseEventBufferSize) {
        // resets the mouse event buffer position
        this->mouseEventBufferPosition = 0;
    }

    // retrieves the current mouse event pointer
    MouseEvent_t *mouseEvent = &this->mouseEventBuffer[this->mouseEventBufferPosition];

    // increments the mouse event buffer position
    this->mouseEventBufferPosition++;

    // returns the mouse event
    return mouseEvent;
}

void Mouse::clearInputEventQueue() {
    // resets the mouse event buffer position
    this->mouseEventBufferPosition = 0;

    // calls the super
    InputDevice::clearInputEventQueue();
}

void Mouse::addMoveEvent(const MousePosition_t &position) {
    // sets the current position
    this->setCurrentPosition(position);

    // in case the move event is not meant to be captured
    // or is temporarily disabled
    if(!this->mouseMoveCapture) {
        // return immediately
        return;
    }

    // creates a new input device event
    InputDeviceEvent_t inputDeviceEvent;

    // retrieves the current mouse event pointer
    MouseEvent_t *mouseEvent = this->getMouseEvent();

    // sets the event type in the mouse event
    mouseEvent->eventType = MOUSE_MOVED_TYPE;

    // sets the position in the mouse event
    mouseEvent->position = position;

    // sets the mouse event in the input device event
    inputDeviceEvent.event = mouseEvent;

    // sets the event type in the input device event
    inputDeviceEvent.eventType = MOUSE;

    // adds the input device event
    this->addInputEvent(inputDeviceEvent);
}

void Mouse::addButtonEvent(const MouseEventType_t &eventType, const MouseButtonCode_t &buttonCode, const MousePosition_t &position) {
     // creates a new input device event
    InputDeviceEvent_t inputDeviceEvent;

    // retrieves the current mouse event pointer
    MouseEvent_t *mouseEvent = this->getMouseEvent();

    // sets the event type in the mouse event
    mouseEvent->eventType = eventType;

    // sets the button code in the mouse event
    mouseEvent->buttonCode = buttonCode;

    // sets the position in the mouse event
    mouseEvent->position = position;

    // sets the mouse event in the input device event
    inputDeviceEvent.event = mouseEvent;

    // sets the event type in the input device event
    inputDeviceEvent.eventType = MOUSE;

    // adds the input device event
    this->addInputEvent(inputDeviceEvent);
}

void Mouse::addButtonDownEvent(const MouseButtonCode_t &buttonCode, const MousePosition_t &position) {
    this->addButtonEvent(BUTTON_DOWN_TYPE, buttonCode, position);
}

void Mouse::addButtonUpEvent(const MouseButtonCode_t &buttonCode, const MousePosition_t &position) {
    this->addButtonEvent(BUTTON_UP_TYPE, buttonCode, position);
}

void Mouse::startMouseMoveCapture() {
    this->setMouseMoveCapture(true);
}

void Mouse::stopMouseMoveCapture() {
    this->setMouseMoveCapture(false);
}

MousePosition_t &Mouse::getCurrentPosition() {
    return this->currentPosition;
}

void Mouse::setCurrentPosition(const MousePosition_t &currentPosition) {
    this->currentPosition = currentPosition;
}

bool Mouse::getMouseMoveCapture() {
    return this->mouseMoveCapture;
}

void Mouse::setMouseMoveCapture(bool mouseMoveCapture) {
    this->mouseMoveCapture = mouseMoveCapture;
}
