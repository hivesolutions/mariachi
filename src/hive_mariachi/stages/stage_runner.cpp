// Hive Mariachi Engine
// Copyright (c) 2008-2020 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#include "stdafx.h"

#include "../system/system_util.h"

#include "stage_runner.h"

using namespace mariachi::stages;

/**
 * Constructor of the class.
 */
StageRunner::StageRunner() {
}

StageRunner::StageRunner(Stage *stage) {
    this->stage = stage;
}

/**
 * Destructor of the class.
 */
StageRunner::~StageRunner() {
}

void StageRunner::start(void *arguments) {
    // sets the running flag
    this->runningFlag = true;

    // starts the stage
    this->stage->start(arguments);

    // iterates while the running flag
    // is set
    while(this->runningFlag) {
        // updates the stage
        this->stage->update(arguments);

        // @todo: temporary hack until sheduller
        SLEEP(100);
    }

    // stops the stage
    this->stage->stop(arguments);
}

void StageRunner::stop(void *arguments) {
    // unsets the running flag
    this->runningFlag = false;
}
