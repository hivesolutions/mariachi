// Hive Mariachi Engine
// Copyright (c) 2008-2020 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../configuration_manager.h"

namespace mariachi {
    namespace configuration {
        class ConfigurationParser {
            private:
                std::string name;

            protected:
                ConfigurationManager *configurationManager;

            public:
                ConfigurationParser();
                ConfigurationParser(ConfigurationManager *configurationManager);
                ~ConfigurationParser();
                inline void initConfigurationManager(ConfigurationManager *configurationManager);
                virtual void parseConfiguration(void *configuration) { };
                virtual void parseResource(void *resource) { };
                virtual std::string &getName() { return this->name; };
                virtual void setName(const std::string &name) { this->name = name; };
        };
    }
}
