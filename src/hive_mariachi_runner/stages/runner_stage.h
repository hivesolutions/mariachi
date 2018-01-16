// Hive Mariachi Engine
// Copyright (c) 2008-2018 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2018 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

namespace mariachi {
    namespace runner {
        /**
        * Class used as the entry point to the process
        * of running a test system for the mariachi
        * game engine.
        */
        class RunnerStage : public stages::Stage {
            private:

            public:
                RunnerStage();
                RunnerStage(Engine *engine);
                RunnerStage(Engine *engine, const std::string &name);
                ~RunnerStage();
                void start(void *arguments);
                void stop(void *arguments);
                void update(void *arguments);
        };
    }
}
