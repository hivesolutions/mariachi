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

#include "../../../exceptions/exceptions.h"

#include "lua_script_engine_util.h"

using namespace mariachi::exceptions;
using namespace mariachi::script::lua;

void mariachi::script::lua::lua_assertargs(lua_State *luaState, unsigned int expectedArgc) {
    // retrieves the number of arguments
    int argc = lua_gettop(luaState);

    if(!argc == expectedArgc) {
        // throws an exception
        throw Exception("Invalid number of arguments");
    }
}

void mariachi::script::lua::lua_assertargsmethod(lua_State *luaState, unsigned int expectedArgc) {
    // calls the arguments assertion with one more argument
    // (the self reference)
    lua_assertargs(luaState, expectedArgc + 1);
}

void *mariachi::script::lua::lua_getself(lua_State *luaState) {
    // loads the self variable table
    lua_pushstring(luaState, LUA_SCRIPT_ENGINE_SELF_VARIABLE);
    lua_gettable(luaState, 1);

    // retrieves the self value
    void *self_value = (void *) lua_tointeger(luaState, -1);

    // pops the temporary value from stack
    lua_pop(luaState, 1);

    // returns the self value
    return self_value;
}

void mariachi::script::lua::lua_setself(lua_State *luaState, void *selfPointer) {
    lua_pushstring(luaState, LUA_SCRIPT_ENGINE_SELF_VARIABLE);
    lua_pushinteger(luaState, (long long) selfPointer);
    lua_settable(luaState, -3);
}

int mariachi::script::lua::lua_hastype(lua_State *luaState, const char *type) {
    lua_pushstring(luaState, type);
    lua_gettable(luaState, -2);

    // retrieves the return value
    int return_value = (int) lua_tointeger(luaState, -1);

    // pops the temporary value from stack
    lua_pop(luaState, 1);

    // returns the return value
    return return_value;
}

void mariachi::script::lua::lua_settype(lua_State *luaState, const char *type) {
    lua_pushstring(luaState, type);
    lua_pushinteger(luaState, 1);
    lua_settable(luaState, -3);
}

/**
 * Sets a mapping name for a string value given as argument.
 *
 * @param luaState The current lua state reference.
 * @param name The mapping name to be used.
 * @param value The string value to be used.
 */
void mariachi::script::lua::lua_setnamestring(lua_State *luaState, const char *name, const char *value) {
    lua_pushstring(luaState, name);
    lua_pushstring(luaState, value);
    lua_settable(luaState, -3);
}

/**
 * Sets a mapping name for a float value given as argument.
 *
 * @param luaState The current lua state reference.
 * @param name The mapping name to be used.
 * @param value The float value to be used.
 */
void mariachi::script::lua::lua_setnamefloat(lua_State *luaState, const char *name, float value) {
    lua_pushstring(luaState, name);
    lua_pushnumber(luaState, value);
    lua_settable(luaState, -3);
}

/**
 * Sets a mapping name for a function value given as argument.
 *
 * @param luaState The current lua state reference.
 * @param name The mapping name to be used.
 * @param value The function value to be used.
 */
void mariachi::script::lua::lua_setnamefunction(lua_State *luaState, const char *name, lua_CFunction value) {
    lua_pushstring(luaState, name);
    lua_pushcfunction(luaState, value);
    lua_settable(luaState, -3);
}
