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

#include "lua_script_engine/_lua_script_engine.h"

#include "lua_script_engine.h"

using namespace mariachi;
using namespace mariachi::script;
using namespace mariachi::script::lua;

/**
 * Constructor of the class.
 */
LuaScriptEngine::LuaScriptEngine() : ScriptEngine() {
}

LuaScriptEngine::LuaScriptEngine(Engine *engine) : ScriptEngine(engine) {
}

/**
 * Destructor of the class.
 */
LuaScriptEngine::~LuaScriptEngine() {
}

void LuaScriptEngine::load(void *arguments) {
    // allocates the defined lua libs
    luaL_reg lualibs[] = { { "base", luaopen_base }, { NULL, NULL } };

    // opens a new lua state
    this->luaState = lua_open();

    // loads the lua base libraries
    luaL_openlibs(this->luaState);

    // opens the defined lua libs
    this->openLuaLibs(lualibs);

    // registers the scrip engine global variable
    lua_pushinteger(luaState, (long long) this);
    lua_setglobal(luaState, LUA_SCRIPT_ENGINE_GLOBAL_VARIABLE);

    // creates a new table and registers the entry point global function
    lua_newtable(luaState);
    lua_setnamefunction(luaState, LUA_SCRIPT_ENGINE_GET_ENGINE_METHOD, LuaMapping::getEngine);
    lua_setglobal(luaState, LUA_SCRIPT_ENGINE_BASE_NAMESPACE);

    // generates the constructors
    LuaMapping::generateConstructors(luaState);
}

void LuaScriptEngine::unload(void *arguments) {
    // destroys the lua state
    lua_close(this->luaState);
}

void LuaScriptEngine::addExtraPaths(std::vector<std::string *> &extraPaths) {
    // loads the global variable
    lua_getglobal(luaState, LUA_SCRIPT_ENGINE_PACKAGE_NAMESPACE);

    // loads the self variable table
    lua_pushstring(luaState, LUA_SCRIPT_ENGINE_PACKAGE_PATH);
    lua_gettable(luaState, 1);

    // retrieves the path value
    const char *packagePath = lua_tostring(luaState, -1);

    // pops the path value
    lua_pop(luaState, 1);

    // creates the new package path string
    std::string newPackagePath(packagePath);

    // retrieves the extra paths iterator
    std::vector<std::string *>::iterator extraPathsIterator = extraPaths.begin();

    // iterates over all the extra paths
    while(extraPathsIterator != extraPaths.end()) {
        // adds the extra path to the new package path
        newPackagePath += LUA_SCRIPT_ENGINE_PACKAGE_PATH_SEPARATOR + **extraPathsIterator;

        // increments the extra paths iterator
        extraPathsIterator++;
    }

    // loads the new path variable table
    lua_pushstring(luaState, LUA_SCRIPT_ENGINE_PACKAGE_PATH);
    lua_pushstring(luaState, newPackagePath.c_str());
    lua_settable(luaState, -3);

    // pops the package global variable
    lua_pop(luaState, 1);
}

bool LuaScriptEngine::runScript(Script_t *script) {
    return true;
}

bool LuaScriptEngine::runScriptFile(const std::string &scriptPath) {
    return !luaL_dofile(this->luaState, scriptPath.c_str());
}

bool LuaScriptEngine::runScriptString(const std::string &scriptString) {
    return !luaL_dostring(this->luaState, scriptString.c_str());
}

std::string LuaScriptEngine::getLastError() {
    return std::string(lua_tostring(this->luaState, -1));
}

int LuaScriptEngine::getObjectReference(void *cObjectReference) {
    return this->objectMap[cObjectReference];
}

void LuaScriptEngine::setObjectReference(void *cObjectReference, int luaObjectIndex) {
    this->objectMap[cObjectReference] = luaObjectIndex;
}

inline void LuaScriptEngine::openLuaLibs(luaL_reg *luaLibs) {
    for(luaL_reg *lib = luaLibs; lib->func != NULL; lib++) {
        lib->func(luaState);
        lua_settop(luaState, 0);
    }
}
