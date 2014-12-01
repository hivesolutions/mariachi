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

// __author__    = Jo�o Magalh�es <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#include "../main/main.h"

namespace mariachi {
    namespace script {
        typedef struct Script_t {
            char *scriptName;
            unsigned int scriptSize;
            void *attributes;
        } Script;

        class ScriptEngine {
            private:

            protected:
                Engine *engine;

            public:
                ScriptEngine();
                ScriptEngine(Engine *engine);
                ~ScriptEngine();
                virtual void load(void *arguments) {};
                virtual void unload(void *arguments) {};
                virtual void addExtraPaths(std::vector<std::string *> &extraPaths) { };
                virtual bool runScript(Script_t *script) { return true; };
                virtual bool runScriptFile(const std::string &scriptPath) { return true; };
                virtual bool runScriptString(const std::string &scriptString) { return true; };
                virtual std::string getLastError() { return NULL; };
                Engine *getEngine();
                void setEngine(Engine *engine);
        };
    }
}
