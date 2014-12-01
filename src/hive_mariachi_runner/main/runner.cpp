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

#include "../../hive_mariachi/mariachi.h"

#include "../stages/runner_stage.h"

using namespace mariachi;
using namespace mariachi::runner;
using namespace mariachi::exceptions;

/**
 * The main entry point of the program.
 *
 * @param argc The number of arguments sent to the program.
 * @param argv The value of the arguments sent to the program.
 * @return The return value of the program.
 */
int main(int argc, char** argv) {
    try {
        // creates a new engine
        Engine engine(argc, argv);

        // creates the runner stage
        RunnerStage runnerStage(&engine, "runner");

        // adds the runner stage to the engine
        engine.addStage(&runnerStage);

        // sets the runner stage as dependent of
        // render, this ensures that sync is made
        // (in case sync is active)
        //engine.addStageDependency("render", "runner");

        // sets the size of the fifo uses to maintain sync
        //runnerStage.setFifoSize(1);

        // starts the engine
        engine.start(NULL);
    } catch(Exception exception) {
        std::cout << "Exception occurred: ";
        std::cout << exception.getMessage();
    }

    // returns zero
    return 0;
}
