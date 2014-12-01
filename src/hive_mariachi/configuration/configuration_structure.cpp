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

#include "configuration_structure.h"

using namespace mariachi::structures;
using namespace mariachi::configuration;

/**
 * Constructor of the class.
 */
ConfigurationStructure::ConfigurationStructure() {
}

/**
 * Destructor of the class.
 */
ConfigurationStructure::~ConfigurationStructure() {
}

void ConfigurationStructure::cleanConfigurationValue(ConfigurationValue_t *configurationValue) {
    // retrieves the configuration value type
    ConfigurationValueType_t configurationValueType = configurationValue->type;

    // switches over the configuration value type
    switch(configurationValueType) {
        // in case is of type int
        case CONFIGURATION_VALUE_INT_TYPE:
            break;

        // in case is of type string
        case CONFIGURATION_VALUE_STRING_TYPE:
            // releases the string value
            free(configurationValue->structure.stringValue);

            break;

        // in case is of type object
        case CONFIGURATION_VALUE_LIST_TYPE:
            // deletes the list value
            delete configurationValue->structure.listValue;

            break;

        // in case is of type object
        case CONFIGURATION_VALUE_OBJECT_TYPE:
            // deletes the object value
            delete configurationValue->structure.objectValue;

            break;
    }

    // releases the configuration value memory
    free(configurationValue);
}

ConfigurationValue_t *ConfigurationStructure::getIntValue(int intValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t));

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_INT_TYPE;

    // sets the int value in the configuration value
    configurationValue->structure.intValue = intValue;

    // returns the configuration value
    return configurationValue;
}

ConfigurationValue_t *ConfigurationStructure::getStringValue(const std::string &stringValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t));

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_STRING_TYPE;

    // sets the string value in the configuration value
    configurationValue->structure.stringValue = new std::string(stringValue);

    // returns the configuration value
    return configurationValue;
}

ConfigurationValue_t *ConfigurationStructure::getBooleanValue(bool booleanValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t));

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_BOOLEAN_TYPE;

    // sets the boolean value in the configuration value
    configurationValue->structure.booleanValue = booleanValue;

    // returns the configuration value
    return configurationValue;
}

ConfigurationValue_t *ConfigurationStructure::getListValue(ConfigurationList *listValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t));

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_LIST_TYPE;

    // sets the list value in the configuration value value
    configurationValue->structure.listValue = listValue;

    // returns the configuration value
    return configurationValue;
}

ConfigurationValue_t *ConfigurationStructure::getObjectValue(ConfigurationMap *objectValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t));

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_OBJECT_TYPE;

    // sets the object value in the configuration value value
    configurationValue->structure.objectValue = objectValue;

    // returns the configuration value
    return configurationValue;
}
