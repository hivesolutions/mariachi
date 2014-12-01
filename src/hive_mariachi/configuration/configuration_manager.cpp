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

#include "../exceptions/exceptions.h"
#include "../structures/structures.h"
#include "parsers/parsers.h"

#include "configuration_manager.h"

using namespace mariachi;
using namespace mariachi::exceptions;
using namespace mariachi::structures;
using namespace mariachi::configuration;

/**
 * Constructor of the class.
 */
ConfigurationManager::ConfigurationManager() : ConfigurationMap() {
}

/**
 * Constructor of the class.
 *
 * @param engine The engine that contains the reference to the configuration manager.
 */
ConfigurationManager::ConfigurationManager(Engine *engine) : ConfigurationMap() {
    this->initEngine(engine);
}

/**
 * Destructor of the class.
 */
ConfigurationManager::~ConfigurationManager() {
}

inline void ConfigurationManager::initEngine(Engine *engine) {
    this->engine = engine;
}

/**
 * Loads the configuration manager with the given arguments.
 *
 * @param arguments The arguments to the load of the configuration manager.
 */
void ConfigurationManager::load(void *arguments) {
    // retrieves the configuration arguments
    ConfigurationArguments_t *configurationArguments = (ConfigurationArguments_t *) arguments;

    // creates a new json configuration parser
    JsonConfigurationParser *jsonConfigurationParser = new JsonConfigurationParser(this);

    // adds the json configuration parser to the configuration parsser list
    configurationParserList.push_back(jsonConfigurationParser);

    // creates the configuration file to be used
    this->configurationFile = new std::fstream(configurationArguments->filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(this->configurationFile->fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + configurationArguments->filePath);
    }

    // seeks to the end of the file
    this->configurationFile->seekg(0, std::fstream::end);

    // get length of file
    std::streamoff configurationFileLength = this->configurationFile->tellg();

    // seeks to the beginning of the file
    this->configurationFile->seekg(0, std::fstream::beg);

    // allocates space for the configuration file data
    char *configurationFileData = (char *) malloc(configurationFileLength);

    // reads the configuration file data
    this->configurationFile->read(configurationFileData, configurationFileLength);

    // creates the file data
    FileData_t fileData = { configurationFileData, configurationFileLength };

    // parses the configuration with the json configuration parser
    jsonConfigurationParser->parseConfiguration(&fileData);
}

/**
 * Unloads the configuration manager with the given arguments.
 *
 * @param arguments The arguments to the unload of the configuration manager.
 */
void ConfigurationManager::unload(void *arguments) {
    // deletes the configuration file
    delete this->configurationFile;
}
