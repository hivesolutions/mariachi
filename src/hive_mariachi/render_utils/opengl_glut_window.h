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

#ifdef MARIACHI_PLATFORM_OPENGL

#ifdef MARIACHI_PLATFORM_MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "opengl_window.h"
#include "../render_adapters/opengl_adapter.h"

#define DEFAULT_OPENGL_GLUT_WINDOW_WIDTH 480

#define DEFAULT_OPENGL_GLUT_WINDOW_HEIGHT 320

#define DEFAULT_OPENGL_GLUT_WINDOW_X 10

#define DEFAULT_OPENGL_GLUT_WINDOW_Y 10

#define DEFAULT_OPENGL_GLUT_WINDOW_TITLE "Mariachi opengl glut window"

void display();
void resizeScene(int windowWidth, int windowHeight);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
void mouseButtonPressed(int button, int state, int x, int y);
void mouseMoved(int x, int y);

namespace mariachi {
    namespace render_utils {
        class OpenglGlutWindow : public OpenglWindow {
            private:
                int window;

            public:
                render_adapters::OpenglAdapter *openglAdapter;
                Engine *engine;

                OpenglGlutWindow();
                ~OpenglGlutWindow();
                void start(void *arguments);
                void stop(void *arguments);
                void loop(void *arguments);
                void fullScreen(void *arguments);
        };
    }
}

#endif
