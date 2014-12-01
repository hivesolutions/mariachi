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

#ifdef MARIACHI_PLATFORM_OPENGL

#include "../devices/input/input.h"

#include "opengl_glut_window.h"

using namespace mariachi::devices;
using namespace mariachi::render_utils;
using namespace mariachi::render_adapters;

OpenglGlutWindow *openglGlutWindow;

/**
 * Constructor of the class.
 */
OpenglGlutWindow::OpenglGlutWindow() : OpenglWindow() {
    // sets the current opengl glut window
    openglGlutWindow = this;
}

/**
 * Destructor of the class.
 */
OpenglGlutWindow::~OpenglGlutWindow() {
}

void OpenglGlutWindow::start(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves the argc value
    int argc = (long long) argumentsMap["argc"];

    // retrieves the argv value
    char **argv = (char **) argumentsMap["argv"];

    // retrieves and sets the opengl adapter
    this->openglAdapter = (OpenglAdapter *) argumentsMap["adapter"];

    // retrieves and sets the engine adapter
    this->engine = (Engine *) argumentsMap["engine"];

    // inializes the glut system
    glutInit(&argc, argv);

    // initializes the display mode
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    // initializes the window size
    glutInitWindowSize(DEFAULT_OPENGL_GLUT_WINDOW_WIDTH, DEFAULT_OPENGL_GLUT_WINDOW_HEIGHT);

    // initializes the window position
    glutInitWindowPosition(DEFAULT_OPENGL_GLUT_WINDOW_X, DEFAULT_OPENGL_GLUT_WINDOW_Y);

    // creates the window
    this->window = glutCreateWindow(DEFAULT_OPENGL_GLUT_WINDOW_TITLE);

    // sets the callback functions
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(resizeScene);
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(specialKeyPressed);
    glutMouseFunc(mouseButtonPressed);
    glutPassiveMotionFunc(mouseMoved);

    // calls the init method in the opengl adapter
    this->openglAdapter->init();
}

void OpenglGlutWindow::stop(void *arguments) {
    // destroys the window
    glutDestroyWindow(this->window);
}

void OpenglGlutWindow::loop(void *arguments) {
    // runs the glut main loop
    glutMainLoop();
}

void OpenglGlutWindow::fullScreen(void *arguments) {
    // sets the screen to full screen
    glutFullScreen();
}

void display() {
#if defined(MARIACHI_ASSYNC_PARALLEL_PROCESSING)
    // calls the display method in the opengl adapter
    openglGlutWindow->openglAdapter->display();

    // swaps the double buffering buffers
    glutSwapBuffers();
#elif defined(MARIACHI_SYNC_PARALLEL_PROCESSING)
    // waits for the fifo to become active
    openglGlutWindow->engine->fifo->wait();

    // enters the critical section
    CRITICAL_SECTION_ENTER(openglGlutWindow->engine->fifo->queueCriticalSection);

    // iterates while the queue is empty and the stop flag is not set
    while(openglGlutWindow->engine->fifo->queue.empty() && !openglGlutWindow->engine->fifo->stopFlag) {
        CONDITION_WAIT(openglGlutWindow->engine->fifo->notEmptyCondition, openglGlutWindow->engine->fifo->queueCriticalSection);
    }

    // calls the display method in the opengl adapter
    openglGlutWindow->openglAdapter->display();

    // swaps the double buffering buffers
    glutSwapBuffers();

    // removes a value from the fifo
    openglGlutWindow->engine->fifo->queue.pop_front();

    // leaves the critical section
    CRITICAL_SECTION_LEAVE(openglGlutWindow->engine->fifo->queueCriticalSection);

    // sends the condition signal
    CONDITION_SIGNAL(openglGlutWindow->engine->fifo->notFullCondition);
#endif
}

void resizeScene(int windowWidth, int windowHeight) {
    // calls the resize scene method in the opengl adapter
    openglGlutWindow->openglAdapter->resizeScene(windowWidth, windowHeight);
}

void keyPressed(unsigned char key, int x, int y) {
    // retrieves the keyboard device
    Keyboard *keyboard = (Keyboard *) openglGlutWindow->engine->getDevice("keyboard");

    // switches in the key value
    switch(key) {
        // in case it's escape key
        case 0x1b:
            keyboard->addKeyDownEvent(KEY_CODE_ESCAPE);
            return;

        // in case it's space key
        case 0x20:
            keyboard->addKeyDownEvent(KEY_CODE_SPACE);
            return;
    }

    // in case it's a numerical key
    if(key >= 0x30 && key <= 0x39) {
        // calculates the key code
        unsigned int keyCode = KEY_CODE_0 + key - 0x30;

        // adds the key down event
        keyboard->addKeyDownEvent((KeyboardKeyCode_t) keyCode);

        // returns immediately
        return;
    }

    // in case it's an uppercase letter key
    if(key >= 0x41 && key <= 0x5a) {
        // calculates the key code
        unsigned int keyCode = KEY_CODE_A + key - 0x41;

        // adds the key down event
        keyboard->addKeyDownEvent((KeyboardKeyCode_t) keyCode);

        // returns immediately
        return;
    }

    // in case it's a lowercase letter key
    if(key >= 0x61 && key <= 0x7a) {
        // calculates the key code
        unsigned int keyCode = KEY_CODE_A + key - 0x61;

        // adds the key down event
        keyboard->addKeyDownEvent((KeyboardKeyCode_t) keyCode);

        // returns immediately
        return;
    }
}

void specialKeyPressed(int key, int x, int y) {
    // retrieves the keyboard device
    Keyboard *keyboard = (Keyboard *) openglGlutWindow->engine->getDevice("keyboard");

    // switches in the key value
    switch(key) {
        case GLUT_KEY_F1:
            keyboard->addKeyDownEvent(KEY_CODE_F1);
            return;

        case GLUT_KEY_F2:
            keyboard->addKeyDownEvent(KEY_CODE_F2);
            return;

        case GLUT_KEY_F3:
            keyboard->addKeyDownEvent(KEY_CODE_F3);
            return;

        case GLUT_KEY_F4:
            keyboard->addKeyDownEvent(KEY_CODE_F4);
            return;

        case GLUT_KEY_F5:
            keyboard->addKeyDownEvent(KEY_CODE_F5);
            return;

        case GLUT_KEY_F6:
            keyboard->addKeyDownEvent(KEY_CODE_F6);
            return;

        case GLUT_KEY_F7:
            keyboard->addKeyDownEvent(KEY_CODE_F7);
            return;

        case GLUT_KEY_F8:
            keyboard->addKeyDownEvent(KEY_CODE_F8);
            return;

        case GLUT_KEY_F9:
            keyboard->addKeyDownEvent(KEY_CODE_F9);
            return;

        case GLUT_KEY_F10:
            keyboard->addKeyDownEvent(KEY_CODE_F10);
            return;

        case GLUT_KEY_F11:
            keyboard->addKeyDownEvent(KEY_CODE_F11);
            return;

        case GLUT_KEY_F12:
            keyboard->addKeyDownEvent(KEY_CODE_F12);
            return;

        case GLUT_KEY_LEFT:
            keyboard->addKeyDownEvent(KEY_CODE_LEFT);
            return;

        case GLUT_KEY_UP:
            keyboard->addKeyDownEvent(KEY_CODE_UP);
            return;

        case GLUT_KEY_RIGHT:
            keyboard->addKeyDownEvent(KEY_CODE_RIGHT);
            return;

        case GLUT_KEY_DOWN:
            keyboard->addKeyDownEvent(KEY_CODE_DOWN);
            return;

        case GLUT_KEY_PAGE_UP:
            keyboard->addKeyDownEvent(KEY_CODE_NEXT);
            return;

        case GLUT_KEY_PAGE_DOWN:
            keyboard->addKeyDownEvent(KEY_CODE_PRIOR);
            return;

        case GLUT_KEY_HOME:
            keyboard->addKeyDownEvent(KEY_CODE_HOME);
            return;

        case GLUT_KEY_END:
            keyboard->addKeyDownEvent(KEY_CODE_END);
            return;

        case GLUT_KEY_INSERT:
            keyboard->addKeyDownEvent(KEY_CODE_INSERT);
            return;
    }
}

void mouseButtonPressed(int button, int state, int x, int y) {
    MouseEventType_t eventType;
    MouseButtonCode_t buttonCode;

    // retrieves the mouse device
    Mouse *mouse = (Mouse *) openglGlutWindow->engine->getDevice("mouse");

    // switches in the button
    switch(button) {
        case GLUT_LEFT_BUTTON:
            buttonCode = BUTTON_CODE_LBUTTON;
            break;
        case GLUT_MIDDLE_BUTTON:
            buttonCode = BUTTON_CODE_MBUTTON;
            break;
        case GLUT_RIGHT_BUTTON:
            buttonCode = BUTTON_CODE_RBUTTON;
            break;
    }

    // switches in the state
    switch(state) {
        case GLUT_DOWN:
            eventType = BUTTON_DOWN_TYPE;
            break;
        case GLUT_UP:
            eventType = BUTTON_UP_TYPE;
            break;
    }

    // creates the mouse position
    MousePosition_t mousePosition = { x, y };

    // adds the button event to the mouse
    mouse->addButtonEvent(eventType, buttonCode, mousePosition);
}

void mouseMoved(int x, int y) {
    // retrieves the mouse device
    Mouse *mouse = (Mouse *) openglGlutWindow->engine->getDevice("mouse");

    // creates the mouse position
    MousePosition_t mousePosition = { x, y };

    // adds the move event to the mouse
    mouse->addMoveEvent(mousePosition);
}

#endif
