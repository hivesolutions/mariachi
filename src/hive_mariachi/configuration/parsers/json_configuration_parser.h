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

#pragma once

#include "../../serialization/json.h"
#include "../configuration_list.h"
#include "../configuration_map.h"
#include "configuration_parser.h"

namespace mariachi {
    namespace configuration {
        class JsonConfigurationParser : public ConfigurationParser {
            private:
                void updateConfigurationMap(ConfigurationMap *configurationMap, const Json::Value &currentNode);
                void updateConfigurationList(ConfigurationList *configurationList, const Json::Value &currentNode);
                structures::ConfigurationValue *createConfigurationValue(const Json::Value &propertyValue);

            public:
                JsonConfigurationParser();
                JsonConfigurationParser(ConfigurationManager *configurationManager);
                ~JsonConfigurationParser();
                void parseConfiguration(void *configuration);
                void parseResource(void *resource);
        };
    }
}
