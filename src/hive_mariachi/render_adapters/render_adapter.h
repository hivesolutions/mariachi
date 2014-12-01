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

#include "../main/engine.h"
#include "../nodes/nodes.h"
#include "../render/render.h"

/**
 * The frame sampling limit, used in
 * the frame rate counting.
 */
#define FRAME_SAMPLING_LIMIT 3.0

/**
 * The reference width to be used in the
 * 2d orthogonal plane.
 */
#define REFERENCE_WIDTH_2D 100.0

/**
 * The reference height to be used in the
 * 2d orthogonal plane.
 */
#define REFERENCE_HEIGHT_2D 100.0

#define NORMAL_LAYOUT 1

#define ROTATED_LAYOUT 2

namespace mariachi {
    namespace render_adapters {
        class RenderAdapter {
            private:
                Engine *engine;
                inline void initLayout();

            protected:
                render::RenderInformation *renderInformation;
                unsigned int frameCount;
                time_t baseClock;
                unsigned int layout;

            public:
                RenderAdapter();
                ~RenderAdapter();
                virtual void start(void *arguments) { };
                virtual void stop(void *arguments) { };
                Engine *getEngine();
                void setEngine(Engine *engine);
                unsigned int getLayout();
                void setLayout(unsigned int layout);
        };
    }
}
