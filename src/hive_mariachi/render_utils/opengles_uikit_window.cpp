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

#ifdef MARIACHI_PLATFORM_IPHONE

#include "../system/system.h"
#include "../tasks/function_caller_task.h"
#include "factories/opengles_uikit_window_factory.h"
#include "opengles_uikit_window.h"

using namespace mariachi::tasks;
using namespace mariachi::render_utils;
using namespace mariachi::render_adapters;

/**
 * Constructor of the class.
 */
OpenglesUikitWindow::OpenglesUikitWindow() : OpenglesWindow() {
}

/**
 * Destructor of the class.
 */
OpenglesUikitWindow::~OpenglesUikitWindow() {
}

void OpenglesUikitWindow::start(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves and sets the opengles adapter
    this->openglesAdapter = (OpenglesAdapter *) argumentsMap["adapter"];

    // retrieves and sets the engine
    this->engine = (Engine *) argumentsMap["engine"];

    FunctionCallerTask *startOpenglesUikitWindowFunctionCallerTask = new FunctionCallerTask();

    // sets the caller function and arguments in the function caller task
    startOpenglesUikitWindowFunctionCallerTask->setCallerFunction(&startOpenglesUikitWindow);
    startOpenglesUikitWindowFunctionCallerTask->setCallerArguments(arguments);

    // adds the function caller task to the engine task list
    this->engine->addTask(startOpenglesUikitWindowFunctionCallerTask);
}

void OpenglesUikitWindow::stop(void *arguments) {
}

void OpenglesUikitWindow::loop(void *arguments) {
    // creates the loop opengles uikit window function caller task
    FunctionCallerTask *loopOpenglesUikitWindowFunctionCallerTask = new FunctionCallerTask();

    // sets the caller function and arguments in the function caller task
    loopOpenglesUikitWindowFunctionCallerTask->setCallerFunction(&loopOpenglesUikitWindow);
    loopOpenglesUikitWindowFunctionCallerTask->setCallerArguments(arguments);

    // adds the function caller task to the engine task list
    this->engine->addTask(loopOpenglesUikitWindowFunctionCallerTask);

    // loops indefinitely
    while(1) {
        // sleeps for ten seconds
        SLEEP(10000);
    }
}

void OpenglesUikitWindow::fullScreen(void *arguments) {
}

#endif
