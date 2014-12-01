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

#include "../../exceptions/exceptions.h"
#include "../../structures/structures.h"

#include "json_configuration_parser.h"

using namespace mariachi::exceptions;
using namespace mariachi::structures;
using namespace mariachi::configuration;

/**
 * Constructor of the class.
 */
JsonConfigurationParser::JsonConfigurationParser() : ConfigurationParser() {
}

/**
 * Constructor of the class.
 *
 * @param configurationManager The configuration manager that contains the reference to the configuration parser.
 */
JsonConfigurationParser::JsonConfigurationParser(ConfigurationManager *configurationManager) : ConfigurationParser(configurationManager) {
}

/**
 * Destructor of the class.
 */
JsonConfigurationParser::~JsonConfigurationParser() {
}

/**
 * Parses the given configuration generating the internal structure.
 *
 * @param configuration The configuration to be parsed.
 */
void JsonConfigurationParser::parseConfiguration(void *configuration) {
    // root json node
    Json::Value rootNode;

    // creates the features object
    Json::Features features;

    // creates the json reader
    Json::Reader reader(features);

    // retrieves the file data from configuration
    FileData_t *fileData = (FileData_t *) configuration;

    // retrieves the reference to the begining of the document
    const char *beginningDocument = (const char *) fileData->data;

    // retrieves the reference to the end of the document
    const char *endDocument = (const char *) &fileData[fileData->size];

    // parses the json file
    bool parsingSuccessful = reader.parse(beginningDocument, endDocument, rootNode);

    // in case the parsing was not succsessful
    if(!parsingSuccessful) {
        // retrieves the error message
        std::string errorMessage = reader.getFormatedErrorMessages();

        // throws a runtime exception
        throw RuntimeException("Failed to parse configuration: " + errorMessage);
    }

    this->updateConfigurationMap(this->configurationManager, rootNode);
}

/**
 * Parses the given resource generating the internal structure.
 *
 * @param resource The resource to be parsed.
 */
void JsonConfigurationParser::parseResource(void *resource) {
}

/**
 * Updates a configuration map with the given json node.
 *
 * @param configurationMap The configuration map to be updated.
 * @param currentNode The node to be used to update the configuration map.
 */
void JsonConfigurationParser::updateConfigurationMap(ConfigurationMap *configurationMap, const Json::Value &currentNode) {
    // retrieves the menber names
    Json::Value::Members currentNodeMembers = currentNode.getMemberNames();

    // retrieves the current node members iterator
    Json::Value::Members::iterator currentNodeMembersIterator = currentNodeMembers.begin();

    // itearates over all the current node members
    while(currentNodeMembersIterator != currentNodeMembers.end()) {
        // retrieves the property name
        std::string &propertyName = *currentNodeMembersIterator;

        // retrieves the property value
        Json::Value propertyValue = currentNode[propertyName];

        // creates the configuration value
        ConfigurationValue *configurationValue = this->createConfigurationValue(propertyValue);

        // sets the propery in the configuration map
        configurationMap->setProperty(propertyName, configurationValue);

        // increments the current node members iterator
        currentNodeMembersIterator++;
    }
}

/**
 * Updates a configuration list with the given json node.
 *
 * @param configurationList The configuration list to be updated.
 * @param currentNode The node to be used to update the configuration list.
 */
void JsonConfigurationParser::updateConfigurationList(ConfigurationList *configurationList, const Json::Value &currentNode) {
    // retrieves the current node size
    unsigned int currentNodeSize = currentNode.size();

    // iterates over the child nodes
    for(unsigned int index = 0; index < currentNodeSize; index++) {
        // retrieves the property value
        Json::Value propertyValue = currentNode[index];

        // creates the configuration value
        ConfigurationValue *configurationValue = this->createConfigurationValue(propertyValue);

        // adds the property to the configuration list
        configurationList->addProperty(configurationValue);
    }
}

ConfigurationValue *JsonConfigurationParser::createConfigurationValue(const Json::Value &propertyValue) {
    // allocates the configuration value pointer
    ConfigurationValue *configurationValue;

    // retrieves the property value type
    Json::ValueType propertyValueType = propertyValue.type();

    // the various property values
    int propertyValueIntValue;
    std::string propertyValueStringValue;
    bool propertyValueBooleanValue;

    // the various composite property values
    ConfigurationList *configurationList;
    ConfigurationMap *configurationMap;

    // switches over the property value type
    switch(propertyValueType) {
        case Json::nullValue:
            break;

        case Json::intValue:
            // retrieves the property value as int
            propertyValueIntValue = propertyValue.asInt();

            configurationValue = ConfigurationStructure::getIntValue(propertyValueIntValue);

            break;

        case Json::uintValue:
            // retrieves the property value as int
            propertyValueIntValue = propertyValue.asUInt();

            configurationValue = ConfigurationStructure::getIntValue(propertyValueIntValue);

            break;

        case Json::realValue:
            break;

        case Json::stringValue:
            // retrieves the property value as string
            propertyValueStringValue = propertyValue.asString();

            configurationValue = ConfigurationStructure::getStringValue(propertyValueStringValue);

            break;

        case Json::booleanValue:
            // retrieves the property value as boolean
            propertyValueBooleanValue = propertyValue.asBool();

            configurationValue = ConfigurationStructure::getBooleanValue(propertyValueBooleanValue);

            break;

        case Json::arrayValue:
            // creates a new configuration list
            configurationList = new ConfigurationList();

            // updates the configuration list with the new "current" node
            this->updateConfigurationList(configurationList, propertyValue);

            configurationValue = ConfigurationStructure::getListValue(configurationList);

            break;

        case Json::objectValue:
            // creates a new configuration map
            configurationMap = new ConfigurationMap();

            // updates the configuration map with the new "current" node
            this->updateConfigurationMap(configurationMap, propertyValue);

            configurationValue = ConfigurationStructure::getObjectValue(configurationMap);

            break;
    }

    // returns the configuration value
    return configurationValue;
}
