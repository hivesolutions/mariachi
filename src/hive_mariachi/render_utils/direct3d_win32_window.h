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

#ifdef MARIACHI_PLATFORM_DIRECT3D

#include "direct3d_window.h"
#include "../render_adapters/direct3d_adapter.h"

#define DEFAULT_DIRECT3D_WIN32_WINDOW_WIDTH 640

#define DEFAULT_DIRECT3D_WIN32_WINDOW_HEIGHT 480

#define DEFAULT_DIRECT3D_WIN32_WINDOW_X 10

#define DEFAULT_DIRECT3D_WIN32_WINDOW_Y 10

#define DEFAULT_DIRECT3D_WIN32_WINDOW_TITLE "Mariachi direct3d win32 window"

#define DIRECT3D_WIN32_WINDOW_CLASS_NAME "direct3d_window"

#define DIRECT3D_WIN32_FULLSCREEN_CLASS_NAME "direct3d_fullscreen"

LRESULT CALLBACK Direct3dWindowProc(HWND handlerWindow, UINT message, WPARAM wParam, LPARAM lParam);

namespace mariachi {
    namespace render_utils {
        class Direct3dWin32Window : public Direct3dWindow {
            private:
                HDC handlerDeviceContext;
                HGLRC handlerOpengl;
                HWND handlerWindow;
                HWND handlerFullScreen;

            public:
                render_adapters::Direct3dAdapter *adapter;

                Direct3dWin32Window();
                ~Direct3dWin32Window();
                void start(void *arguments);
                void stop(void *arguments);
                void loop(void *arguments);
                void fullScreen(void *arguments);
                HWND getHandlerWindow();
                void createWindow(char* title, int x, int y, int width, int height);
        };
    }
}

#endif
