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

#ifdef MARIACHI_PLATFORM_MACOSX

#include "../system/system.h"
#include "../tasks/function_caller_task.h"
#include "factories/opengl_cocoa_window_factory.h"
#include "opengl_cocoa_window.h"

using namespace mariachi::tasks;
using namespace mariachi::render_utils;
using namespace mariachi::render_adapters;

OpenglCocoaWindow::OpenglCocoaWindow() : OpenglWindow() {
}

OpenglCocoaWindow::~OpenglCocoaWindow() {
}

void OpenglCocoaWindow::start(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves and sets the opengl adapter
    this->openglAdapter = (OpenglAdapter *) argumentsMap["adapter"];

    // retrieves and sets the engine
    this->engine = (Engine *) argumentsMap["engine"];

    FunctionCallerTask *startOpenglCocoaWindowFunctionCallerTask = new FunctionCallerTask();

    // sets the caller function and arguments in the function caller task
    startOpenglCocoaWindowFunctionCallerTask->setCallerFunction(&startOpenglCocoaWindow);
    startOpenglCocoaWindowFunctionCallerTask->setCallerArguments(arguments);

    // adds the function caller task to the engine task list
    this->engine->addTask(startOpenglCocoaWindowFunctionCallerTask);
}

void OpenglCocoaWindow::stop(void *arguments) {
}

void OpenglCocoaWindow::loop(void *arguments) {
    // creates the loop opengl cocoa window function caller task
    FunctionCallerTask *loopOpenglCocoaWindowFunctionCallerTask = new FunctionCallerTask();

    // sets the caller function and arguments in the function caller task
    loopOpenglCocoaWindowFunctionCallerTask->setCallerFunction(&loopOpenglCocoaWindow);
    loopOpenglCocoaWindowFunctionCallerTask->setCallerArguments(arguments);

    // adds the function caller task to the engine task list
    this->engine->addTask(loopOpenglCocoaWindowFunctionCallerTask);

    // loops indefinitely
    while (1) {
        // sleeps for ten seconds
        SLEEP(10000);
    }
}

void OpenglCocoaWindow::fullScreen(void *arguments) {
}

#endif
