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

#ifndef MARIACHI_STAGE_DUMMY
#define MARIACHI_STAGE_DUMMY true
#endif

#include "stage.h"

namespace mariachi {
    namespace stages {
        /**
         * Just a dummy stage for testing purposes.
         * This stage should be used carefully. It may affect the stability
         * of the program or system.
         */
        class DummyStage : public Stage {
            private:

            public:
                DummyStage();
                DummyStage(Engine *engine);
                DummyStage(Engine *engine, const std::string &name);
                ~DummyStage();
                void start(void *arguments);
                void stop(void *arguments);
                void update(void *arguments);
        };
    }
}
