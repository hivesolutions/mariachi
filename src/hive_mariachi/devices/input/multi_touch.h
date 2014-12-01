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

#pragma once

#include "input_device.h"

#define DEFAULT_MULTI_TOUCH_EVENT_BUFFER_SIZE 10240

#define DEFAULT_MULTI_TOUCH_TOUCHES_MOVED_CAPTURE true

namespace mariachi {
    namespace devices {
        typedef enum TouchType_t {
            TOUCH_BEGAN_TYPE = 1,
            TOUCH_MOVED_TYPE,
            TOUCH_STATIONARY_TYPE,
            TOUCH_ENDED_TYPE,
            TOUCH_CANCELLED_TYPE
        } TouchType;

        typedef struct TouchPosition_t {
            unsigned short touchPositionX;
            unsigned short touchPositionY;
        } TouchPosition;

        typedef struct Touch_t {
            TouchType_t touchType;
            TouchPosition_t position;
        } Touch;

        typedef struct MultiTouchEvent_t {
            Touch_t *touches;
            unsigned int numberTouches;
        } MultiTouchEvent;

        class MultiTouch : public InputDevice {
            private:
                /**
                 * The buffer of multi touch events.
                 */
                MultiTouchEvent_t *multiTouchEventBuffer;

                /**
                 * The size of the events buffer.
                 */
                unsigned int multiTouchEventBufferSize;

                /**
                 * The current position in the event buffer.
                 */
                unsigned int multiTouchEventBufferPosition;

                /**
                 * Flag that controls the capture of the touches moved event.
                 */
                bool touchesMovedCapture;

            public:
                MultiTouch();
                MultiTouch(unsigned int multiTouchEventBufferSize);
                ~MultiTouch();
                inline void initMultiTouchEventBuffer();
                inline void initTouchesMovedCapture();
                inline MultiTouchEvent_t *getMultiTouchEvent();
                virtual void clearInputEventQueue();
                void addTouchesMovedEvent(Touch_t *touches, const unsigned int numberTouches);
                void startTouchesMovedCapture();
                void stopTouchesMovedCapture();
                bool getTouchesMovedCapture();
                void setTouchesMovedCapture(bool touchesMovedCapture);
        };
    }
}
