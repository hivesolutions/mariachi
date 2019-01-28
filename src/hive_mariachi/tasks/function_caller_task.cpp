// Hive Mariachi Engine
// Copyright (c) 2008-2019 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2019 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#include "stdafx.h"

#include "function_caller_task.h"

using namespace mariachi::tasks;

/**
 * Constructor of the class.
 */
FunctionCallerTask::FunctionCallerTask() : Task() {
    this->initCallerArguments();
}

FunctionCallerTask::FunctionCallerTask(const std::string &name) : Task(name) {
    this->initCallerArguments();
}

/**
 * Destructor of the class.
 */
FunctionCallerTask::~FunctionCallerTask() {
}

inline void FunctionCallerTask::initCallerArguments() {
    this->callerArguments = NULL;
}

void FunctionCallerTask::start(void *parameters) {
    this->callerFunction(this->callerArguments);
}

void FunctionCallerTask::stop(void *parameters) {
}

void (*FunctionCallerTask::getCallerFunction())(void *) {
    return this->callerFunction;
}

void FunctionCallerTask::setCallerFunction(void (*callerFunction)(void *)) {
    this->callerFunction = callerFunction;
}

void *FunctionCallerTask::getCallerArguments() {
    return this->callerArguments;
}

void FunctionCallerTask::setCallerArguments(void *callerArguments) {
    this->callerArguments = callerArguments;
}
