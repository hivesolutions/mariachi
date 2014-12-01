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

extern "C" {
    #include "../../../lib/liblua/src/lua.h"
    #include "../../../lib/liblua/src/lualib.h"
    #include "../../../lib/liblua/src/lauxlib.h"
}

#include "script_engine.h"

#define LUA_SCRIPT_ENGINE_PACKAGE_NAMESPACE "package"

#define LUA_SCRIPT_ENGINE_PACKAGE_PATH "path"

#define LUA_SCRIPT_ENGINE_PACKAGE_PATH_SEPARATOR ";"

#define LUA_SCRIPT_ENGINE_GLOBAL_VARIABLE "lua_script_engine"

#define LUA_SCRIPT_ENGINE_BASE_NAMESPACE "mariachi"

#define LUA_SCRIPT_ENGINE_GET_ENGINE_METHOD "get_engine"

namespace mariachi {
    namespace script {
        class LuaScriptEngine : public ScriptEngine {
            private:
                lua_State *luaState;
                std::map<void *, int> objectMap;

            public:
                LuaScriptEngine();
                LuaScriptEngine(Engine *engine);
                ~LuaScriptEngine();
                void load(void *arguments);
                void unload(void *arguments);
                void addExtraPaths(std::vector<std::string *> &extraPaths);
                bool runScript(Script_t *script);
                bool runScriptFile(const std::string &scriptPath);
                bool runScriptString(const std::string &scriptString);
                std::string getLastError();
                int getObjectReference(void *cObjectReference);
                void setObjectReference(void *cObjectReference, int luaObjectIndex);
                inline void openLuaLibs(luaL_reg *luaLibs);
        };
    }
}
