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

#ifdef MARIACHI_PLATFORM_DIRECT3D

#include "../exceptions/exceptions.h"

#include "direct3d_win32_window.h"

using namespace mariachi::exceptions;
using namespace mariachi::render_utils;
using namespace mariachi::render_adapters;

Direct3dWin32Window *direct3dWin32Window;

Direct3dWin32Window::Direct3dWin32Window() : Direct3dWindow() {
    // sets the current direct3d win32 window
    direct3dWin32Window = this;
}

Direct3dWin32Window::~Direct3dWin32Window() {
}

void Direct3dWin32Window::start(void *arguments) {
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    this->adapter = (Direct3dAdapter *) argumentsMap["adapter"];

    // creates the window
    this->createWindow(DEFAULT_DIRECT3D_WIN32_WINDOW_TITLE, DEFAULT_DIRECT3D_WIN32_WINDOW_X, DEFAULT_DIRECT3D_WIN32_WINDOW_Y, DEFAULT_DIRECT3D_WIN32_WINDOW_WIDTH, DEFAULT_DIRECT3D_WIN32_WINDOW_HEIGHT);

    // enables the direct3d
    //this->enableDirect3d();

    // calls the init method in the direct3d adapter
    this->adapter->init();
}

void Direct3dWin32Window::stop(void *arguments) {
}

void Direct3dWin32Window::loop(void *arguments) {
    // allocates the message
    MSG  message;

    // shows the window
    ShowWindow(this->handlerWindow, SW_SHOWDEFAULT);

    while(1) {
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // calls the display method in the open gl adapter
        this->adapter->display();
    }

    ReleaseDC(this->handlerWindow, this->handlerDeviceContext);
    DestroyWindow(this->handlerWindow);
}

void Direct3dWin32Window::fullScreen(void *arguments) {
    // creates the full screen
    //this->createFullScreen(DEFAULT_WIN32_WINDOW_TITLE, DEFAULT_WIN32_WINDOW_WIDTH, DEFAULT_WIN32_WINDOW_HEIGHT);

    // @todo: implement this behaviour
}

HWND Direct3dWin32Window::getHandlerWindow() {
    return this->handlerWindow;
}

void Direct3dWin32Window::createWindow(char* title, int x, int y, int width, int height) {
    HWND handlerWindow;
    WNDCLASSEX windowClassEx;

    static HINSTANCE handlerInstance = 0;

    // avoid duplicate registration of class
    if(!handlerInstance) {
        handlerInstance = GetModuleHandle(NULL);

        ZeroMemory(&windowClassEx, sizeof(WNDCLASSEX));

        windowClassEx.cbSize = sizeof(WNDCLASSEX);
        windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
        windowClassEx.lpfnWndProc = Direct3dWindowProc;
        windowClassEx.cbClsExtra = 0;
        windowClassEx.cbWndExtra = 0;
        windowClassEx.hInstance = handlerInstance;
        windowClassEx.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        windowClassEx.hIconSm = NULL;
        windowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClassEx.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        windowClassEx.lpszMenuName  = NULL;
        windowClassEx.lpszClassName = DIRECT3D_WIN32_WINDOW_CLASS_NAME;

        if(!RegisterClassEx(&windowClassEx)) {
            // throws a runtime exception
            throw RuntimeException("Cannot register window class");
        }
    }

    // creates the window and retrieves the handler
    handlerWindow = CreateWindowEx(NULL, DIRECT3D_WIN32_WINDOW_CLASS_NAME, title, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, width, height, NULL, NULL, handlerInstance, NULL);

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

LRESULT CALLBACK Direct3dWindowProc(HWND handlerWindow, UINT message, WPARAM wParam, LPARAM lParam)  {
    static PAINTSTRUCT paintStructure;

    switch(message) {
        case WM_CREATE:
            return 0;

        case WM_PAINT:
            BeginPaint(handlerWindow, &paintStructure);
            EndPaint(handlerWindow, &paintStructure);
            return 0;

        case WM_SIZE:
            direct3dWin32Window->adapter->resizeScene(LOWORD(lParam), HIWORD(lParam));

            // forces the repaint
            PostMessage(handlerWindow, WM_PAINT, 0, 0);

            return 0;

        case WM_CLOSE:
            PostQuitMessage(0);

            return 0;

        default:
            return DefWindowProc(handlerWindow, message, wParam, lParam);
    }
}

#endif
