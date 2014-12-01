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

#include "keyboard.h"

using namespace mariachi::devices;

/**
 * Constructor of the class.
 */
Keyboard::Keyboard() : InputDevice() {
    // sets the keyboard event buffer size as the default one
    this->keyboardEventBufferSize = DEFAULT_KEYBOARD_EVENT_BUFFER_SIZE;

    // initializes the keyboard event buffer
    this->initKeyboardEventBuffer();
}

Keyboard::Keyboard(unsigned int keyboardEventBufferSize) {
    // sets the keyboard event buffer size
    this->keyboardEventBufferSize = keyboardEventBufferSize;

    // initializes the keyboard event buffer
    this->initKeyboardEventBuffer();
}

/**
 * Destructor of the class.
 */
Keyboard::~Keyboard() {
    // releases the keybord event buffer
    free(this->keyboardEventBuffer);
}

inline void Keyboard::initKeyboardEventBuffer() {
    // allocates space for the keyboard event buffer
    this->keyboardEventBuffer = (KeyboardEvent_t *) malloc(sizeof(KeyboardEvent_t) * this->keyboardEventBufferSize);

    // resets the keyboard event buffer position
    this->keyboardEventBufferPosition = 0;
}

inline KeyboardEvent_t *Keyboard::getKeyboardEvent() {
    // in case the keyboard event buffer position is greater or equal than
    // the keyboard event buffer size
    if(this->keyboardEventBufferPosition >= this->keyboardEventBufferSize) {
        // resets the keyboard event buffer position
        this->keyboardEventBufferPosition = 0;
    }

    // retrieves the current keyboard event pointer
    KeyboardEvent_t *keyboardEvent = &this->keyboardEventBuffer[this->keyboardEventBufferPosition];

    // increments the keyboard event buffer position
    this->keyboardEventBufferPosition++;

    // returns the mouse event
    return keyboardEvent;
}

void Keyboard::clearInputEventQueue() {
    // resets the keyboard event buffer position
    this->keyboardEventBufferPosition = 0;

    // calls the super
    InputDevice::clearInputEventQueue();
}

void Keyboard::addKeyEvent(const KeyboardEventType_t &eventType, const KeyboardKeyCode_t &keyCode) {
    // in case the keyboard event buffer position is greater or equal than
    // the keyboard event buffer size
    if(this->keyboardEventBufferPosition >= this->keyboardEventBufferSize) {
        // resets the keyboard event buffer position
        this->keyboardEventBufferPosition = 0;
    }

    // creates a new input device event
    InputDeviceEvent_t inputDeviceEvent;

    // retrieves the current keyboard event pointer
    KeyboardEvent_t *keyboardEvent = &this->keyboardEventBuffer[this->keyboardEventBufferPosition];

    // sets the event type in the keyboard event
    keyboardEvent->eventType = eventType;

    // sets the key code in the keyboard event
    keyboardEvent->keyCode = keyCode;

    // sets the keyboard event in the input device event
    inputDeviceEvent.event = keyboardEvent;

    // sets the event type in the input device event
    inputDeviceEvent.eventType = KEYBOARD;

    // adds the input device event
    this->addInputEvent(inputDeviceEvent);

    // increments the keyboard event buffer position
    this->keyboardEventBufferPosition++;
}

void Keyboard::addKeyDownEvent(const KeyboardKeyCode_t &keyCode) {
    this->addKeyEvent(KEY_DOWN_TYPE, keyCode);
}
