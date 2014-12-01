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

#define DEFAULT_KEYBOARD_EVENT_BUFFER_SIZE 1024

namespace mariachi {
    namespace devices {
        typedef enum KeyboardEventType_t {
            KEY_DOWN_TYPE = 1,
            KEY_UP_TYPE,
            KEY_PRESSED_TYPE
        } KeyboardEventType;

        typedef enum KeyboardKeyCode_t {
            KEY_CODE_LBUTTON,
            KEY_CODE_RBUTTON,
            KEY_CODE_CANCEL,
            KEY_CODE_MBUTTON,
            KEY_CODE_BACK,
            KEY_CODE_TAB,
            KEY_CODE_CLEAR,
            KEY_CODE_RETURN,
            KEY_CODE_SHIFT,
            KEY_CODE_CONTROL,
            KEY_CODE_MENU,
            KEY_CODE_PAUSE,
            KEY_CODE_CAPITAL,
            KEY_CODE_ESCAPE,
            KEY_CODE_SPACE,
            KEY_CODE_PRIOR,
            KEY_CODE_NEXT,
            KEY_CODE_END,
            KEY_CODE_HOME,
            KEY_CODE_LEFT,
            KEY_CODE_UP,
            KEY_CODE_RIGHT,
            KEY_CODE_DOWN,
            KEY_CODE_SELECT,
            KEY_CODE_PRINT,
            KEY_CODE_EXECUTE,
            KEY_CODE_SNAPSHOT,
            KEY_CODE_INSERT,
            KEY_CODE_DELETE,
            KEY_CODE_HELP,
            KEY_CODE_NUMPAD0,
            KEY_CODE_NUMPAD1,
            KEY_CODE_NUMPAD2,
            KEY_CODE_NUMPAD3,
            KEY_CODE_NUMPAD4,
            KEY_CODE_NUMPAD5,
            KEY_CODE_NUMPAD6,
            KEY_CODE_NUMPAD7,
            KEY_CODE_NUMPAD8,
            KEY_CODE_NUMPAD9,
            KEY_CODE_SEPARATOR,
            KEY_CODE_SUBTRACT,
            KEY_CODE_DECIMAL,
            KEY_CODE_DIVIDE,
            KEY_CODE_F1,
            KEY_CODE_F2,
            KEY_CODE_F3,
            KEY_CODE_F4,
            KEY_CODE_F5,
            KEY_CODE_F6,
            KEY_CODE_F7,
            KEY_CODE_F8,
            KEY_CODE_F9,
            KEY_CODE_F10,
            KEY_CODE_F11,
            KEY_CODE_F12,
            KEY_CODE_F13,
            KEY_CODE_F14,
            KEY_CODE_F15,
            KEY_CODE_F16,
            KEY_CODE_F17,
            KEY_CODE_F18,
            KEY_CODE_F19,
            KEY_CODE_F20,
            KEY_CODE_F21,
            KEY_CODE_F22,
            KEY_CODE_F23,
            KEY_CODE_F24,
            KEY_CODE_NUMLOCK,
            KEY_CODE_SCROLL,
            KEY_CODE_LSHIFT,
            KEY_CODE_RSHIFT,
            KEY_CODE_LCONTROL,
            KEY_CODE_RCONTROL,
            KEY_CODE_LMENU,
            KEY_CODE_RMENU,
            KEY_CODE_PLAY,
            KEY_CODE_ZOOM,
            KEY_CODE_0,
            KEY_CODE_1,
            KEY_CODE_2,
            KEY_CODE_3,
            KEY_CODE_4,
            KEY_CODE_5,
            KEY_CODE_6,
            KEY_CODE_7,
            KEY_CODE_8,
            KEY_CODE_9,
            KEY_CODE_A,
            KEY_CODE_B,
            KEY_CODE_C,
            KEY_CODE_D,
            KEY_CODE_E,
            KEY_CODE_F,
            KEY_CODE_G,
            KEY_CODE_H,
            KEY_CODE_I,
            KEY_CODE_J,
            KEY_CODE_K,
            KEY_CODE_L,
            KEY_CODE_M,
            KEY_CODE_N,
            KEY_CODE_O,
            KEY_CODE_P,
            KEY_CODE_Q,
            KEY_CODE_R,
            KEY_CODE_S,
            KEY_CODE_T,
            KEY_CODE_U,
            KEY_CODE_V,
            KEY_CODE_W,
            KEY_CODE_X,
            KEY_CODE_Y,
            KEY_CODE_Z
        } KeyboardKeyCode;

        typedef struct KeyboardEvent_t {
            KeyboardEventType_t eventType;
            KeyboardKeyCode_t keyCode;
        } KeyboardEvent;

        class Keyboard : public InputDevice {
            private:
                KeyboardEvent_t *keyboardEventBuffer;
                unsigned int keyboardEventBufferSize;
                unsigned int keyboardEventBufferPosition;

            public:
                Keyboard();
                Keyboard(unsigned int keyboardEventBufferSize);
                ~Keyboard();
                inline void initKeyboardEventBuffer();
                inline KeyboardEvent_t *getKeyboardEvent();
                virtual void clearInputEventQueue();
                void addKeyEvent(const KeyboardEventType_t &eventType, const KeyboardKeyCode_t &keyCode);
                void addKeyDownEvent(const KeyboardKeyCode_t &keyCode);
        };
    }
}
