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
    #include "../../../../../lib/liblua/src/lua.h"
}

#include "../../../main/main.h"
#include "../../../nodes/nodes.h"
#include "../../../structures/structures.h"

#include "../util/lua_script_engine_util.h"
#include "../../lua_script_engine.h"

#define lua_mariachi_validate_reference(return_value, luaState, value, node_type, upper_function)\
    if((return_value = upper_function(luaState, value)) || (return_value = !lua_hastype(luaState, node_type)))

namespace mariachi {
    namespace script {
        namespace lua {
            typedef bool (*LuaConstructor_t)(lua_State *luaState, void *value);

            typedef enum LuaType_t {
                LUA_SCRIPT_ENGINE_LIST_TYPE_INT = 1,
                LUA_SCRIPT_ENGINE_MAP_TYPE_INT,
                LUA_SCRIPT_ENGINE_VECTOR_TYPE_INT,
                LUA_SCRIPT_ENGINE_NODE_TYPE_INT,
                LUA_SCRIPT_ENGINE_CUBE_NODE_TYPE_INT,
                LUA_SCRIPT_ENGINE_SCENE_NODE_TYPE_INT,
                LUA_SCRIPT_ENGINE_COORDINATE_3D_TYPE_INT
            } LuaType;

            typedef struct LuaTypeInformation_t {
                unsigned int intType;
                LuaConstructor_t constructor;
                lua_CFunction luaConstructor;
            } LuaTypeInformation;

            class LuaObject {
                private:

                public:
                    static bool allocate(lua_State *luaState, void *value);
                    static int castAs(lua_State *luaState);
                    static int typeId(lua_State *luaState);
            };

            class LuaMapping {
                private:

                public:
                    static bool getReference(lua_State *luaState, void *value);
                    static int getEngine(lua_State *luaState);
                    static LuaScriptEngine *getScriptEngine(lua_State *luaState);
                    static void generateConstructors(lua_State *luaState);
                    static unsigned int getIntType(const char *charType);
                    static LuaConstructor_t getConstructor(const char *charType);
                    static void constructTypeInformationMap();
            };
        }
    }
}

#include "base/base.h"
#include "main/main.h"
#include "nodes/nodes.h"
#include "structures/structures.h"
