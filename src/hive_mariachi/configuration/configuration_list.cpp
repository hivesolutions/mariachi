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

#include "configuration_list.h"

using namespace mariachi::structures;
using namespace mariachi::configuration;

/**
 * Constructor of the class.
 */
ConfigurationList::ConfigurationList() : ConfigurationStructure() {
}

/**
 * Destructor of the class.
 */
ConfigurationList::~ConfigurationList() {
    // cleans the list
    this->cleanList();
}

/**
 * Cleans the internal list removing all the memory references
 * in the internal structure.
 */
inline void ConfigurationList::cleanList() {
    // retrieves the configuration list size
    unsigned int configurationListSize = this->configurationList.size();

    // iterates over the configuration list
    for(unsigned int index = 0; index < configurationListSize; index++) {
        // removes the property in the given index
        this->removeProperty(index);
    }
}

/**
 * Retrieves a property from configuration.
 *
 * @param index The index of the property to be retrieved.
 * @return The property value in the given index.
 */
ConfigurationValue_t *ConfigurationList::getProperty(unsigned int index) {
    return this->configurationList[index];
}

void ConfigurationList::addProperty(ConfigurationValue_t *value) {
    this->configurationList.push_back(value);
}

void ConfigurationList::removeProperty(unsigned int index) {
    // retrieves the configuration value
    ConfigurationValue_t *configurationValue = this->configurationList[index];

    // removes the configuration value from the configuration list
    this->configurationList.erase(this->configurationList.begin() + index - 1);

    // cleans the configuration value
    this->cleanConfigurationValue(configurationValue);
}

void ConfigurationList::addIntProperty(const std::string &key, int intValue) {
     // creates the configuration value for the int value
    ConfigurationValue_t *configurationValue = this->getIntValue(intValue);

    // adds the property
    this->addProperty(configurationValue);
}

void ConfigurationList::addStringProperty(const std::string &key, const std::string &stringValue) {
     // creates the configuration value for the string value
    ConfigurationValue_t *configurationValue = this->getStringValue(stringValue);

    // adds the property
    this->addProperty(configurationValue);
}

void ConfigurationList::addBooleanProperty(const std::string &key, bool booleanValue) {
     // creates the configuration value for the boolean value
    ConfigurationValue_t *configurationValue = this->getBooleanValue(booleanValue);

    // adds the property
    this->addProperty(configurationValue);
}

void ConfigurationList::addObjectProperty(const std::string &key, ConfigurationMap *objectValue) {
     // creates the configuration value for the object value
    ConfigurationValue_t *configurationValue = this->getObjectValue(objectValue);

    // adds the property
    this->addProperty(configurationValue);
}

std::vector<std::string *> ConfigurationList::getAsStringVector() {
    // allocates the string vector value
    std::vector<std::string *> stringVectorValue;

    // retrieves the configuration list iterator
    std::vector<ConfigurationValue_t *>::iterator configurationListIterator = this->configurationList.begin();

    while(configurationListIterator != this->configurationList.end()) {
        // retrieves the current configuration value
        ConfigurationValue_t *currentConfigurationValue = *configurationListIterator;

        // adds the string value to the string vector value
        stringVectorValue.push_back(currentConfigurationValue->structure.stringValue);

        // increments the configuration list iterator
        configurationListIterator++;
    }

    return stringVectorValue;
}
