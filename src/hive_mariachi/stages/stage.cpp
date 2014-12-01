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

#include "stage.h"

using namespace mariachi;
using namespace mariachi::stages;

/**
 * Constructor of the class.
 */
Stage::Stage() : Module() {
    this->initThread();
}

/**
 * Constructor of the class.
 *
 * @param engine The engine that contains the reference to the stage.
 */
Stage::Stage(Engine *engine) {
    this->initThread();
    this->initEngine(engine);
}

/**
 * Constructor of the class.
 *
 * @param engine The engine that contains the reference to the stage.
 * @param name The name to be used to reference the stage.
 */
Stage::Stage(Engine *engine, const std::string &name) {
    this->initThread();
    this->initEngine(engine);
    this->initName(name);
}

/**
 * Destructor of the class.
 */
Stage::~Stage() {
}

inline void Stage::initThread() {
    this->thread = false;
}

inline void Stage::initEngine(Engine *engine) {
    this->engine = engine;
}

inline void Stage::initName(std::string name) {
    this->name = name;
}

/**
 * Starts the stage.
 *
 * @param arguments The arguments for the stage start.
 */
void Stage::start(void *arguments) {
    Module::start(arguments);
}

/**
 * Stops the stage.
 *
 * @param arguments The arguments for the stage stop.
 */
void Stage::stop(void *arguments) {
    Module::stop(arguments);
}

/**
 * Updates the stage.
 *
 * @param arguments The arguments for the stage update.
 */
void Stage::update(void *arguments) {
}

/**
 * Formats the logger value to include extra information related
 * with the current stage.
 *
 * @param value The base value to be formatted.
 * @return The final formatted value.
 */
std::string Stage::formatLoggerValue(const std::string &value) {
    // allocates the formatted value string
    std::string formattedValue;

    // allocates the formatted value string stream
    std::stringstream formattedValueStream;

    // puts the stage identification in the formatted value stream
    formattedValueStream << "[" + this->name + "] ";

    // puts the value in the formatted value stream
    formattedValueStream << value;

    // retrieves the formatted value from the formatted value stream
    formattedValue = formattedValueStream.rdbuf()->str();

    // returns the formatted value
    return formattedValue;
}

/**
 * Retrieves if the stage requires a thread of it's own.
 *
 * @return If the stage requires a thread of it's own.
 */
bool Stage::requiresThread() {
    return this->thread;
}

/**
 * Retrieves the name.
 *
 * @return The name.
 */
std::string &Stage::getName() {
    return this->name;
}

/**
 * Sets the name.
 *
 * @param name The name.
 */
void Stage::setName(std::string &name) {
    this->name = name;
}

/**
 * Retrieves the engine.
 *
 * @return The engine.
 */
Engine *Stage::getEngine() {
    return this->engine;
}

/**
 * Sets the engine.
 *
 * @param engine The engine.
 */
void Stage::setEngine(Engine *engine) {
    this->engine = engine;
}
