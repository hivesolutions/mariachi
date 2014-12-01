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

#include "../util/util.h"
#include "configuration_map.h"

using namespace mariachi::util;
using namespace mariachi::structures;
using namespace mariachi::configuration;

/**
 * Constructor of the class.
 */
ConfigurationMap::ConfigurationMap() : ConfigurationStructure() {
}

/**
 * Destructor of the class.
 */
ConfigurationMap::~ConfigurationMap() {
    // cleans the map
    this->cleanMap();
}

/**
 * Cleans the internal map removing all the memory references
 * in the internal structure.
 */
inline void ConfigurationMap::cleanMap() {
    // retrieves the configuration map iterator
    std::map<std::string, ConfigurationValue_t *>::iterator configurationMapIterator = this->configurationMap.begin();

    // iterates over the configuration map
    while(configurationMapIterator != this->configurationMap.end()) {
        // retrieves the current key
        std::string key = configurationMapIterator->first;

        // removes the property with the given key
        this->removeProperty(key);
    }
}

/**
 * Retrieves a property from configuration.
 *
 * @param key The key used to identify the property.
 * @return The property value for the given key.
 */
ConfigurationValue_t *ConfigurationMap::_getProperty(const std::string &key) {
    return this->configurationMap[key];
}

/**
 * Retrieves a property from configuration using the full qualified name.
 *
 * @param key The key used to identify the property.
 * @return The property value for the given key.
 */
ConfigurationValue_t *ConfigurationMap::getProperty(const std::string &key) {
    // allocates the tokens vector
    std::vector<std::string> tokens;

    // splits the key in tokens
    StringUtil::tokenize(key, tokens, "/");

    // retrieves the tokens iterator
    std::vector<std::string>::iterator tokensIterator = tokens.begin();

    // sets the initial configuration map (this)
    ConfigurationMap *configurationMap = this;

    // the configuration value used to store the properties
    ConfigurationValue_t *configurationValue;

    // sets the is first flag
    bool isFirst = true;

    // iteretas over all the tokens
    while(tokensIterator != tokens.end()) {
        // in case it's the first iteration
        if(isFirst) {
            // unsets the is first flag
            isFirst = false;
        }
        else {
            // in case the type of the configuration value is not object
            if(configurationValue->type != CONFIGURATION_VALUE_OBJECT_TYPE) {
                return NULL;
            }

            // sets the new configuration map
            configurationMap = (ConfigurationMap *) configurationValue->structure.objectValue;
        }

        // retrieves the current token
        std::string token = *tokensIterator;

        // retrieves the property
        configurationValue = configurationMap->_getProperty(token);

        // in case the configuration value is not valid
        if(!configurationValue) {
            return NULL;
        }

        // increments the tokens iterator
        tokensIterator++;
    }

    // returns the configuration value
    return configurationValue;
}

/**
 * Sets the propery with the given key.
 *
 * @param key The key used to identify the property.
 * @param value The property value to be used.
 */
void ConfigurationMap::setProperty(const std::string &key, ConfigurationValue_t *value) {
    this->configurationList.push_back(value);
    this->configurationMap[key] = value;
}

void ConfigurationMap::removeProperty(const std::string &key) {
    // retrieves the configuration value
    ConfigurationValue_t *configurationValue = this->configurationMap[key];

    // removes the configuration value from the configuration map
    this->configurationMap.erase(key);

    // removes the configuration value from the configuration list
    this->configurationList.remove(configurationValue);

    // cleans the configuration value
    this->cleanConfigurationValue(configurationValue);
}

void ConfigurationMap::setIntProperty(const std::string &key, int intValue) {
    // creates the configuration value for the int value
    ConfigurationValue_t *configurationValue = this->getIntValue(intValue);

    // sets the property
    this->setProperty(key, configurationValue);
}

void ConfigurationMap::setStringProperty(const std::string &key, const std::string &stringValue) {
    // creates the configuration value for the string value
    ConfigurationValue_t *configurationValue = this->getStringValue(stringValue);

    // sets the property
    this->setProperty(key, configurationValue);
}

void ConfigurationMap::setBooleanProperty(const std::string &key, bool booleanValue) {
    // creates the configuration value for the boolean value
    ConfigurationValue_t *configurationValue = this->getBooleanValue(booleanValue);

    // sets the property
    this->setProperty(key, configurationValue);
}

void ConfigurationMap::setObjectProperty(const std::string &key, ConfigurationMap *objectValue) {
    // creates the configuration value for the object value
    ConfigurationValue_t *configurationValue = this->getObjectValue(objectValue);

    // sets the property
    this->setProperty(key, configurationValue);
}
