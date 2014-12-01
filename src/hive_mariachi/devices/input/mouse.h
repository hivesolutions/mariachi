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

#include "input_device.h"

#define DEFAULT_MOUSE_EVENT_BUFFER_SIZE 10240

#define DEFAULT_MOUSE_MOVE_CAPTURE true

namespace mariachi {
    namespace devices {
        typedef enum MouseEventType_t {
            MOUSE_MOVED_TYPE = 1,
            BUTTON_DOWN_TYPE,
            BUTTON_UP_TYPE,
            BUTTON_PRESSED_TYPE
        } MouseEventType;

        typedef enum MouseButtonCode_t {
            BUTTON_CODE_LBUTTON = 1,
            BUTTON_CODE_RBUTTON,
            BUTTON_CODE_MBUTTON
        } MouseButtonCode;

        typedef struct MousePosition_t {
            unsigned short mousePositionX;
            unsigned short mousePositionY;
        } MousePosition;

        typedef struct MouseEvent_t {
            MouseEventType_t eventType;
            MouseButtonCode_t buttonCode;
            MousePosition_t position;
        } MouseEvent;

        class Mouse : public InputDevice {
            private:
                MouseEvent_t *mouseEventBuffer;
                unsigned int mouseEventBufferSize;
                unsigned int mouseEventBufferPosition;

                /**
                 * The current mouse position.
                 */
                MousePosition_t currentPosition;

                /**
                 * Flag that controls the capture of the mouse move event.
                 */
                bool mouseMoveCapture;

            public:
                Mouse();
                Mouse(unsigned int keyboardEventBufferSize);
                ~Mouse();
                inline void initMouseEventBuffer();
                inline void initMouseMoveCapture();
                inline MouseEvent *getMouseEvent();
                virtual void clearInputEventQueue();
                void addMoveEvent(const MousePosition_t &position);
                void addButtonEvent(const MouseEventType_t &eventType, const MouseButtonCode_t &buttonCode, const MousePosition_t &position);
                void addButtonDownEvent(const MouseButtonCode_t &buttonCode, const MousePosition_t &position);
                void addButtonUpEvent(const MouseButtonCode_t &buttonCode, const MousePosition_t &position);
                void startMouseMoveCapture();
                void stopMouseMoveCapture();
                MousePosition_t &getCurrentPosition();
                void setCurrentPosition(const MousePosition_t &currentPosition);
                bool getMouseMoveCapture();
                void setMouseMoveCapture(bool mouseMoveCapture);
        };
    }
}
