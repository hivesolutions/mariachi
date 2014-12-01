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

#ifdef MARIACHI_PLATFORM_WIN32

#include "../exceptions/exceptions.h"
#include "../devices/input/input.h"

#include "opengl_win32_window.h"

using namespace mariachi::devices;
using namespace mariachi::exceptions;
using namespace mariachi::render_utils;
using namespace mariachi::render_adapters;

OpenglWin32Window *openglWin32Window;

OpenglWin32Window::OpenglWin32Window() : OpenglWindow() {
    // sets the current opengl win32 window
    openglWin32Window = this;
}

OpenglWin32Window::~OpenglWin32Window() {
}

void OpenglWin32Window::start(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves and sets the opengl adapter
    this->openglAdapter = (OpenglAdapter *) argumentsMap["adapter"];

    // retrieves and sets the engine
    this->engine = (Engine *) argumentsMap["engine"];

    // creates the window
    this->createWindow(DEFAULT_OPENGL_WIN32_WINDOW_TITLE, DEFAULT_OPENGL_WIN32_WINDOW_X, DEFAULT_OPENGL_WIN32_WINDOW_Y, DEFAULT_OPENGL_WIN32_WINDOW_WIDTH, DEFAULT_OPENGL_WIN32_WINDOW_HEIGHT);

    // enables the open gl
    this->enableOpengl();

    // calls the init method in the opengl adapter
    this->openglAdapter->init();
}

void OpenglWin32Window::stop(void *arguments) {
    // unsets the running flag
    this->runningFlag = false;
}

void OpenglWin32Window::loop(void *arguments) {
    // allocates the message
    MSG message;

    // shows the window
    ShowWindow(this->handlerWindow, SW_SHOWDEFAULT);

    // sets the running flag
    this->runningFlag = true;

    // iterates while the running flag is active
    while(this->runningFlag) {
        // retrieves the message
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            // translates the message
            TranslateMessage(&message);

            // dispatches the message
            DispatchMessage(&message);
        }
#if defined(MARIACHI_ASSYNC_PARALLEL_PROCESSING)
        // calls the display method in the opengl adapter
        this->openglAdapter->display();

        // swaps the buffers
        SwapBuffers(openglWin32Window->handlerDeviceContext);
#elif defined(MARIACHI_SYNC_PARALLEL_PROCESSING)
        // waits for the fifo to become active
        this->engine->fifo->wait();

        // enters the critical section
        CRITICAL_SECTION_ENTER(this->engine->fifo->queueCriticalSection);

        // iterates while the queue is empty and the stop flag is not set
        while(this->engine->fifo->queue.empty() && !this->engine->fifo->stopFlag) {
            CONDITION_WAIT(this->engine->fifo->notEmptyCondition, this->engine->fifo->queueCriticalSection);
        }

        // calls the display method in the opengl adapter
        this->openglAdapter->display();

        // swaps the buffers
        SwapBuffers(openglWin32Window->handlerDeviceContext);

        // removes a value from the fifo
        this->engine->fifo->queue.pop_front();

        // leaves the critical section
        CRITICAL_SECTION_LEAVE(this->engine->fifo->queueCriticalSection);

        // sends the condition signal
        CONDITION_SIGNAL(this->engine->fifo->notFullCondition);
#endif
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(this->handlerWindow, this->handlerDeviceContext);
    wglDeleteContext(this->handlerOpengl);
    DestroyWindow(this->handlerWindow);
}

void OpenglWin32Window::fullScreen(void *arguments) {
    // creates the full screen
    this->createFullScreen(DEFAULT_OPENGL_WIN32_WINDOW_TITLE, DEFAULT_OPENGL_WIN32_WINDOW_WIDTH, DEFAULT_OPENGL_WIN32_WINDOW_HEIGHT);

    // @todo: implement this behaviour
}

void OpenglWin32Window::createWindow(char* title, int x, int y, int width, int height) {
    HWND handlerWindow;
    WNDCLASSEX windowClassEx;

    static HINSTANCE handlerInstance = 0;

    // avoid duplicate registration of class
    if(!handlerInstance) {
        handlerInstance = GetModuleHandle(NULL);

        ZeroMemory(&windowClassEx, sizeof(WNDCLASSEX));

        windowClassEx.cbSize = sizeof(WNDCLASSEX);
        windowClassEx.style =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClassEx.lpfnWndProc = OpenglWin32WindowProc;
        windowClassEx.cbClsExtra = 0;
        windowClassEx.cbWndExtra = 0;
        windowClassEx.hInstance = handlerInstance;
        windowClassEx.hIcon = NULL;
        windowClassEx.hIconSm = LoadIcon(windowClassEx.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        windowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClassEx.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        windowClassEx.lpszMenuName = NULL;
        windowClassEx.lpszClassName = OPENGL_WIN32_WINDOW_CLASS_NAME;

        if(!RegisterClassEx(&windowClassEx)) {
            // throws a runtime exception
            throw RuntimeException("Cannot register window class");
        }
    }

    // creates the window and retrieves the handler
    handlerWindow = CreateWindowEx(NULL, OPENGL_WIN32_WINDOW_CLASS_NAME, title, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, width, height, NULL, NULL, handlerInstance, NULL);

    // in case the window handler is invalid
    if(handlerWindow == NULL) {
        // throws a runtime exception
        throw RuntimeException("Cannot create a window");
    }

    // sets the window handler
    this->handlerWindow = handlerWindow;

    // retrieves the handler device context for the window
    this->handlerDeviceContext = GetDC(this->handlerWindow);
}

void OpenglWin32Window::createFullScreen(char *title, int width, int height) {
    HWND handlerFullScreen;
    WNDCLASSEX windowClassEx;

    static HINSTANCE handlerInstance = 0;

    // avoid duplicate registration of class
    if(!handlerInstance) {
        handlerInstance = GetModuleHandle(NULL);

        ZeroMemory(&windowClassEx, sizeof(WNDCLASSEX));

        windowClassEx.cbSize = sizeof(WNDCLASSEX);
        windowClassEx.style =  CS_HREDRAW | CS_VREDRAW;;
        windowClassEx.lpfnWndProc = OpenglWin32WindowProc;
        windowClassEx.cbClsExtra = 0;
        windowClassEx.cbWndExtra = 0;
        windowClassEx.hInstance = handlerInstance;
        windowClassEx.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        windowClassEx.hIconSm = NULL;
        windowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClassEx.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        windowClassEx.lpszMenuName = NULL;
        windowClassEx.lpszClassName = OPENGL_WIN32_FULLSCREEN_CLASS_NAME;

        if(!RegisterClassEx(&windowClassEx)) {
            // throws a runtime exception
            throw RuntimeException("Cannot register fullscreen class");
        }
    }

    // creates the full screen window and retrieves the handler
    handlerFullScreen = CreateWindowEx(NULL, OPENGL_WIN32_FULLSCREEN_CLASS_NAME, title, WS_EX_TOPMOST | WS_POPUP, 0, 0, width, height, NULL, NULL, handlerInstance, NULL);

    // in case the full screen window handler is invalid
    if(handlerFullScreen == NULL) {
        // throws a runtime exception
        throw RuntimeException("Cannot create a window fullscreen");
    }

    // sets the full screen window handler
    this->handlerWindow = handlerFullScreen;

    // retrieves the handler device context for the window
    this->handlerDeviceContext = GetDC(this->handlerWindow);
}

void OpenglWin32Window::enableOpengl() {
    // allocates the pixel format
    int pixelfFormat;

    // allocates the pixel format descriptor
    PIXELFORMATDESCRIPTOR pixelFormatDescriptor;

    // cleans the bits
    memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));

    pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
    pixelFormatDescriptor.nVersion = 1;
    pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

    // sets the color to 32 bits
    pixelFormatDescriptor.cColorBits = 32;

    // sets the depth to 32 bits
    pixelFormatDescriptor.cDepthBits = 16;

    // sets the stencil to 32 bits
    pixelFormatDescriptor.cStencilBits = 32;

    // chooses the pixel format
    pixelfFormat = ChoosePixelFormat(this->handlerDeviceContext, &pixelFormatDescriptor);

    if(pixelfFormat == 0) {
        // throws a runtime exception
        throw RuntimeException("Cannot find a suitable pixel format");
    }

    if(SetPixelFormat(this->handlerDeviceContext, pixelfFormat, &pixelFormatDescriptor) == false) {
        // throws a runtime exception
        throw RuntimeException("Cannot set the pixel format specified");
    }

    // describes the pixel format
    SetPixelFormat(this->handlerDeviceContext, pixelfFormat, &pixelFormatDescriptor);

    // creates the opengl (wgl) handler (context)
    this->handlerOpengl = wglCreateContext(this->handlerDeviceContext);

    // assigns the opengl handler to the handler device context of the window
    wglMakeCurrent(this->handlerDeviceContext, this->handlerOpengl);
}

void OpenglWin32Window::changeDisplayResolution(int width, int height, int colorBits) {
    DEVMODE deviceModeScreenSettings;

    // resets the device mode screen settings
    memset(&deviceModeScreenSettings, 0, sizeof(deviceModeScreenSettings));

    deviceModeScreenSettings.dmSize = sizeof(deviceModeScreenSettings);
    deviceModeScreenSettings.dmPelsWidth = width;
    deviceModeScreenSettings.dmPelsHeight = height;
    deviceModeScreenSettings.dmBitsPerPel = colorBits;
    deviceModeScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if(ChangeDisplaySettings(&deviceModeScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        // throws a runtime exception
        throw RuntimeException("Display mode change failed");
    }
}

LRESULT CALLBACK OpenglWin32WindowProc(HWND handlerWindow, UINT message, WPARAM wParam, LPARAM lParam)  {
    // the paint structure to be used
    static PAINTSTRUCT paintStructure;

    // the devices to be updated
    Keyboard *keyboard;
    Mouse *mouse;
    MousePosition mousePosition;

    // allocates space for the mouse position
    unsigned int mousePositionX, mousePositionY;

    switch(message) {
        case WM_CREATE:
            return 0;

        case WM_PAINT:
            // begins the paint of the window
            BeginPaint(handlerWindow, &paintStructure);

            // ends the paint of the window
            EndPaint(handlerWindow, &paintStructure);

            return 0;

        case WM_SIZE:
            // calls the resize scene in the opengl adapter
            openglWin32Window->openglAdapter->resizeScene(LOWORD(lParam), HIWORD(lParam));

            // forces the repaint
            PostMessage(handlerWindow, WM_PAINT, 0, 0);

            return 0;

        case WM_CLOSE:
            // sends the post quite message
            PostQuitMessage(0);

            return 0;

        case WM_CHAR:
            return 0;

        case WM_LBUTTONDOWN:
            // retrieves the mouse position
            mousePositionX = LOWORD(lParam);
            mousePositionY = HIWORD(lParam);

            // retrieves the mouse device
            mouse = (Mouse *) openglWin32Window->engine->getDevice("mouse");

            // constructs the mouse position structure
            mousePosition.mousePositionX = mousePositionX;
            mousePosition.mousePositionY = mousePositionY;

            mouse->addButtonUpEvent(BUTTON_CODE_LBUTTON, mousePosition);

            return 0;

        case WM_LBUTTONUP:
            // retrieves the mouse position
            mousePositionX = LOWORD(lParam);
            mousePositionY = HIWORD(lParam);

            // retrieves the mouse device
            mouse = (Mouse *) openglWin32Window->engine->getDevice("mouse");

            // constructs the mouse position structure
            mousePosition.mousePositionX = mousePositionX;
            mousePosition.mousePositionY = mousePositionY;

            mouse->addButtonDownEvent(BUTTON_CODE_LBUTTON, mousePosition);

            return 0;

        case WM_RBUTTONDOWN:
            // retrieves the mouse position
            mousePositionX = LOWORD(lParam);
            mousePositionY = HIWORD(lParam);

            // retrieves the mouse device
            mouse = (Mouse *) openglWin32Window->engine->getDevice("mouse");

            // constructs the mouse position structure
            mousePosition.mousePositionX = mousePositionX;
            mousePosition.mousePositionY = mousePositionY;

            mouse->addButtonUpEvent(BUTTON_CODE_RBUTTON, mousePosition);

            return 0;

        case WM_RBUTTONUP:
            // retrieves the mouse position
            mousePositionX = LOWORD(lParam);
            mousePositionY = HIWORD(lParam);

            // retrieves the mouse device
            mouse = (Mouse *) openglWin32Window->engine->getDevice("mouse");

            // constructs the mouse position structure
            mousePosition.mousePositionX = mousePositionX;
            mousePosition.mousePositionY = mousePositionY;

            mouse->addButtonDownEvent(BUTTON_CODE_RBUTTON, mousePosition);

            return 0;

        case WM_MOUSEMOVE:
            // retrieves the mouse position
            mousePositionX = LOWORD(lParam);
            mousePositionY = HIWORD(lParam);

            // retrieves the mouse device
            mouse = (Mouse *) openglWin32Window->engine->getDevice("mouse");

            // constructs the mouse position structure
            mousePosition.mousePositionX = mousePositionX;
            mousePosition.mousePositionY = mousePositionY;

            mouse->addMoveEvent(mousePosition);

            return 0;

        case WM_KEYDOWN:
            // retrieves the keyboard device
            keyboard = (Keyboard *) openglWin32Window->engine->getDevice("keyboard");

            // switchs over the "virtual" key code
            switch(wParam) {
                case VK_LBUTTON:
                    keyboard->addKeyDownEvent(KEY_CODE_LBUTTON);
                    return 0;

                case VK_RBUTTON:
                    keyboard->addKeyDownEvent(KEY_CODE_RBUTTON);
                    return 0;

                case VK_CANCEL:
                    keyboard->addKeyDownEvent(KEY_CODE_CANCEL);
                    return 0;

                case VK_MBUTTON:
                    keyboard->addKeyDownEvent(KEY_CODE_MBUTTON);
                    return 0;

                case VK_BACK:
                    keyboard->addKeyDownEvent(KEY_CODE_BACK);
                    return 0;

                case VK_TAB:
                    keyboard->addKeyDownEvent(KEY_CODE_TAB);
                    return 0;

                case VK_CLEAR:
                    keyboard->addKeyDownEvent(KEY_CODE_CLEAR);
                    return 0;

                case VK_RETURN:
                    keyboard->addKeyDownEvent(KEY_CODE_RETURN);
                    return 0;

                case VK_SHIFT:
                    keyboard->addKeyDownEvent(KEY_CODE_SHIFT);
                    return 0;

                case VK_CONTROL:
                    keyboard->addKeyDownEvent(KEY_CODE_CONTROL);
                    return 0;

                case VK_MENU:
                    keyboard->addKeyDownEvent(KEY_CODE_MENU);
                    return 0;

                case VK_PAUSE:
                    keyboard->addKeyDownEvent(KEY_CODE_PAUSE);
                    return 0;

                case VK_CAPITAL:
                    keyboard->addKeyDownEvent(KEY_CODE_CAPITAL);
                    return 0;

                case VK_ESCAPE:
                    keyboard->addKeyDownEvent(KEY_CODE_ESCAPE);
                    return 0;

                case VK_SPACE:
                    keyboard->addKeyDownEvent(KEY_CODE_SPACE);
                    return 0;

                case VK_PRIOR:
                    keyboard->addKeyDownEvent(KEY_CODE_PRIOR);
                    return 0;

                case VK_NEXT:
                    keyboard->addKeyDownEvent(KEY_CODE_NEXT);
                    return 0;

                case VK_END:
                    keyboard->addKeyDownEvent(KEY_CODE_END);
                    return 0;

                case VK_HOME:
                    keyboard->addKeyDownEvent(KEY_CODE_HOME);
                    return 0;

                case VK_LEFT:
                    keyboard->addKeyDownEvent(KEY_CODE_LEFT);
                    return 0;

                case VK_UP:
                    keyboard->addKeyDownEvent(KEY_CODE_UP);
                    return 0;

                case VK_RIGHT:
                    keyboard->addKeyDownEvent(KEY_CODE_RIGHT);
                    return 0;

                case VK_DOWN:
                    keyboard->addKeyDownEvent(KEY_CODE_DOWN);
                    return 0;

                case VK_SELECT:
                    keyboard->addKeyDownEvent(KEY_CODE_SELECT);
                    return 0;

                case VK_PRINT:
                    keyboard->addKeyDownEvent(KEY_CODE_PRINT);
                    return 0;

                case VK_EXECUTE:
                    keyboard->addKeyDownEvent(KEY_CODE_EXECUTE);
                    return 0;

                case VK_SNAPSHOT:
                    keyboard->addKeyDownEvent(KEY_CODE_SNAPSHOT);
                    return 0;

                case VK_INSERT:
                    keyboard->addKeyDownEvent(KEY_CODE_INSERT);
                    return 0;

                case VK_DELETE:
                    keyboard->addKeyDownEvent(KEY_CODE_DELETE);
                    return 0;

                case VK_HELP:
                    keyboard->addKeyDownEvent(KEY_CODE_HELP);
                    return 0;

                case VK_NUMPAD0:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD0);
                    return 0;

                case VK_NUMPAD1:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD1);
                    return 0;

                case VK_NUMPAD2:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD2);
                    return 0;

                case VK_NUMPAD3:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD3);
                    return 0;

                case VK_NUMPAD4:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD4);
                    return 0;

                case VK_NUMPAD5:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD5);
                    return 0;

                case VK_NUMPAD6:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD6);
                    return 0;

                case VK_NUMPAD7:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD7);
                    return 0;

                case VK_NUMPAD8:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD8);
                    return 0;

                case VK_NUMPAD9:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMPAD9);
                    return 0;

                case VK_SEPARATOR:
                    keyboard->addKeyDownEvent(KEY_CODE_SEPARATOR);
                    return 0;

                case VK_SUBTRACT:
                    keyboard->addKeyDownEvent(KEY_CODE_SUBTRACT);
                    return 0;

                case VK_DECIMAL:
                    keyboard->addKeyDownEvent(KEY_CODE_DECIMAL);
                    return 0;

                case VK_DIVIDE:
                    keyboard->addKeyDownEvent(KEY_CODE_DIVIDE);
                    return 0;

                case VK_F1:
                    keyboard->addKeyDownEvent(KEY_CODE_F1);
                    return 0;

                case VK_F2:
                    keyboard->addKeyDownEvent(KEY_CODE_F2);
                    return 0;

                case VK_F3:
                    keyboard->addKeyDownEvent(KEY_CODE_F3);
                    return 0;

                case VK_F4:
                    keyboard->addKeyDownEvent(KEY_CODE_F4);
                    return 0;

                case VK_F5:
                    keyboard->addKeyDownEvent(KEY_CODE_F5);
                    return 0;

                case VK_F6:
                    keyboard->addKeyDownEvent(KEY_CODE_F6);
                    return 0;

                case VK_F7:
                    keyboard->addKeyDownEvent(KEY_CODE_F7);
                    return 0;

                case VK_F8:
                    keyboard->addKeyDownEvent(KEY_CODE_F8);
                    return 0;

                case VK_F9:
                    keyboard->addKeyDownEvent(KEY_CODE_F9);
                    return 0;

                case VK_F10:
                    keyboard->addKeyDownEvent(KEY_CODE_F10);
                    return 0;

                case VK_F11:
                    keyboard->addKeyDownEvent(KEY_CODE_F11);
                    return 0;

                case VK_F12:
                    keyboard->addKeyDownEvent(KEY_CODE_F12);
                    return 0;

                case VK_F13:
                    keyboard->addKeyDownEvent(KEY_CODE_F13);
                    return 0;

                case VK_F14:
                    keyboard->addKeyDownEvent(KEY_CODE_F14);
                    return 0;

                case VK_F15:
                    keyboard->addKeyDownEvent(KEY_CODE_F15);
                    return 0;

                case VK_F16:
                    keyboard->addKeyDownEvent(KEY_CODE_F16);
                    return 0;

                case VK_F17:
                    keyboard->addKeyDownEvent(KEY_CODE_F17);
                    return 0;

                case VK_F18:
                    keyboard->addKeyDownEvent(KEY_CODE_F18);
                    return 0;

                case VK_F19:
                    keyboard->addKeyDownEvent(KEY_CODE_F19);
                    return 0;

                case VK_F20:
                    keyboard->addKeyDownEvent(KEY_CODE_F20);
                    return 0;

                case VK_F21:
                    keyboard->addKeyDownEvent(KEY_CODE_F21);
                    return 0;

                case VK_F22:
                    keyboard->addKeyDownEvent(KEY_CODE_F22);
                    return 0;

                case VK_F23:
                    keyboard->addKeyDownEvent(KEY_CODE_F23);
                    return 0;

                case VK_F24:
                    keyboard->addKeyDownEvent(KEY_CODE_F24);
                    return 0;

                case VK_NUMLOCK:
                    keyboard->addKeyDownEvent(KEY_CODE_NUMLOCK);
                    return 0;

                case VK_SCROLL:
                    keyboard->addKeyDownEvent(KEY_CODE_SCROLL);
                    return 0;

                case VK_LSHIFT:
                    keyboard->addKeyDownEvent(KEY_CODE_LSHIFT);
                    return 0;

                case VK_RSHIFT:
                    keyboard->addKeyDownEvent(KEY_CODE_RSHIFT);
                    return 0;

                case VK_LCONTROL:
                    keyboard->addKeyDownEvent(KEY_CODE_LCONTROL);
                    return 0;

                case VK_RCONTROL:
                    keyboard->addKeyDownEvent(KEY_CODE_RCONTROL);
                    return 0;

                case VK_LMENU:
                    keyboard->addKeyDownEvent(KEY_CODE_LMENU);
                    return 0;

                case VK_RMENU:
                    keyboard->addKeyDownEvent(KEY_CODE_RMENU);
                    return 0;

                case VK_PLAY:
                    keyboard->addKeyDownEvent(KEY_CODE_PLAY);
                    return 0;

                case VK_ZOOM:
                    keyboard->addKeyDownEvent(KEY_CODE_ZOOM);
                    return 0;
            }

            // in case it's a numerical key
            if(wParam >= 0x30 && wParam <= 0x39) {
                // calculates the key code
                unsigned int keyCode = KEY_CODE_0 + wParam - 0x30;

                // adds the key down event
                keyboard->addKeyDownEvent((KeyboardKeyCode_t) keyCode);

                // returns zero
                return 0;
            }

            // in case it's a letter key
            if(wParam >= 0x41 && wParam <= 0x5A) {
                // calculates the key code
                unsigned int keyCode = KEY_CODE_A + wParam - 0x41;

                // adds the key down event
                keyboard->addKeyDownEvent((KeyboardKeyCode_t) keyCode);

                // returns zero
                return 0;
            }

            // returns zero
            return 0;

        default:
            return DefWindowProc(handlerWindow, message, wParam, lParam);
    }
}

#endif
