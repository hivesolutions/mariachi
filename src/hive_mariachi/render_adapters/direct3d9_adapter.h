// Hive Mariachi Engine
// Copyright (c) 2008-2016 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2016 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#ifdef MARIACHI_PLATFORM_DIRECT3D

#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>

#include "../render_utils/direct3d_window.h"
#include "direct3d_adapter.h"

namespace mariachi {
    namespace render_adapters {
        class Direct3d9Adapter : public Direct3dAdapter {
            private:
                /**
                 * The main direct3d window.
                 */
                render_utils::Direct3dWindow *window;

                /**
                 * The pointer to the direct3d interface.
                 */
                LPDIRECT3D9 direct3d;

                /**
                 * The pointer to the device class.
                 */
                LPDIRECT3DDEVICE9 direct3dDevice;

                /**
                 * The pointer to the triangle vertex buffer.
                 */
                LPDIRECT3DVERTEXBUFFER9 triangleVertexBuffer;

                inline time_t clockSeconds();
                inline void updateFrameRate();

            public:
                Direct3d9Adapter();
                ~Direct3d9Adapter();
                void start(void *arguments);
                void stop(void *arguments);
                void init();
                void clean();
                void display();
                void resizeScene(int windowWidth, int windowHeight);
                void keyPressed(unsigned char key, int x, int y);
        };
    }
}

#endif
