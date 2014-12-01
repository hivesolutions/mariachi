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

#include "../structures/configuration.h"
#include "configuration_structure.h"

namespace mariachi {
    namespace configuration {
        class ConfigurationMap : public ConfigurationStructure {
            private:
                std::map<std::string, structures::ConfigurationValue_t *> configurationMap;
                std::list<structures::ConfigurationValue_t *> configurationList;

            public:
                ConfigurationMap();
                ~ConfigurationMap();
                inline void cleanMap();
                structures::ConfigurationValue_t *_getProperty(const std::string &key);
                structures::ConfigurationValue_t *getProperty(const std::string &key);
                void setProperty(const std::string &key, structures::ConfigurationValue_t *value);
                void removeProperty(const std::string &key);
                void setIntProperty(const std::string &key, int intValue);
                void setStringProperty(const std::string &key, const std::string &stringValue);
                void setBooleanProperty(const std::string &key, bool booleanValue);
                void setObjectProperty(const std::string &key, ConfigurationMap *objectValue);
        };
    }
}
