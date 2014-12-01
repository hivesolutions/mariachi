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

// __author__    = Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "multi_touch.h"

using namespace mariachi::devices;

/**
 * Constructor of the class.
 */
MultiTouch::MultiTouch() : InputDevice() {
    // sets the multi touch event buffer size as the default one
    this->multiTouchEventBufferSize = DEFAULT_MULTI_TOUCH_EVENT_BUFFER_SIZE;

    // initializes the multi touch event buffer
    this->initMultiTouchEventBuffer();

    // initializes the touches moved capture
    this->initTouchesMovedCapture();
}

MultiTouch::MultiTouch(unsigned int multiTouchEventBufferSize) {
    // sets the multi touch event buffer size
    this->multiTouchEventBufferSize = multiTouchEventBufferSize;

    // initializes the multi touch event buffer
    this->initMultiTouchEventBuffer();

    // initializes the multi touch move capture
    this->initTouchesMovedCapture();
}

MultiTouch::~MultiTouch() {
    // releases the multi touch event buffer
    free(this->multiTouchEventBuffer);
}

inline void MultiTouch::initMultiTouchEventBuffer() {
    // allocates space for the multi touch event buffer
    this->multiTouchEventBuffer = (MultiTouchEvent_t *) malloc(sizeof(MultiTouchEvent_t) * this->multiTouchEventBufferSize);

    // resets the multi touch event buffer position
    this->multiTouchEventBufferPosition = 0;
}

inline void MultiTouch::initTouchesMovedCapture() {
    this->touchesMovedCapture = DEFAULT_MULTI_TOUCH_TOUCHES_MOVED_CAPTURE;
}

inline MultiTouchEvent_t *MultiTouch::getMultiTouchEvent() {
    // in case the multi touch event buffer position is greater or equal than
    // the multi touch event buffer size
    if(this->multiTouchEventBufferPosition >= this->multiTouchEventBufferSize) {
        // resets the multi touch event buffer position
        this->multiTouchEventBufferPosition = 0;
    }

    // retrieves the current multi touch event pointer
    MultiTouchEvent_t *multiTouchEvent = &this->multiTouchEventBuffer[this->multiTouchEventBufferPosition];

    // increments the multi touch event buffer position
    this->multiTouchEventBufferPosition++;

    // returns the multi touch event
    return multiTouchEvent;
}

void MultiTouch::clearInputEventQueue() {
    // resets the multi touch event buffer position
    this->multiTouchEventBufferPosition = 0;

    // calls the super
    InputDevice::clearInputEventQueue();
}

void MultiTouch::addTouchesMovedEvent(Touch_t *touches, const unsigned int numberTouches) {
    // in case the moved event is not meant to be captured
    // or is temporarily disabled
    if(!this->touchesMovedCapture) {
        // returns immediately
        return;
    }

    // creates a new input device event
    InputDeviceEvent_t inputDeviceEvent;

    // retrieves the current multi touch event pointer
    MultiTouchEvent_t *multiTouchEvent = this->getMultiTouchEvent();

    // adds the touches to the event
    multiTouchEvent->touches = touches;

    // sets the number of touches of the event
    multiTouchEvent->numberTouches = numberTouches;

    // sets the multi touch event in the input device event
    inputDeviceEvent.event = multiTouchEvent;

    // sets the event type in the input device event
    inputDeviceEvent.eventType = MULTI_TOUCH;

    // adds the input device event
    this->addInputEvent(inputDeviceEvent);
}

void MultiTouch::startTouchesMovedCapture() {
    this->setTouchesMovedCapture(true);
}

void MultiTouch::stopTouchesMovedCapture() {
    this->setTouchesMovedCapture(false);
}

bool MultiTouch::getTouchesMovedCapture() {
    return this->touchesMovedCapture;
}

void MultiTouch::setTouchesMovedCapture(bool touchesMovedCapture) {
    this->touchesMovedCapture = touchesMovedCapture;
}
