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

#include "lua_script_engine_mapping.h"

using namespace mariachi::script;
using namespace mariachi::script::lua;

/**
 * The lua type information map used to map a string type
 * with the lua type information of the type.
 */
std::map<std::string, LuaTypeInformation_t> lua_typeinformationmap;

/**
 * Base object creation or retrieval function. It allocates a new object (table) reference
 * for the c++ object in case none exists. In case one reeference exists returns
 * it.
 *
 * @param luaState The current lua state reference.
 * @param value The pointer to the c++ object.
 * @return If a new reference was created.
 */
bool LuaMapping::getReference(lua_State *luaState, void *value) {
    // retrieves the lua script engine
    LuaScriptEngine *luaScriptEngine = LuaMapping::getScriptEngine(luaState);

    // allocates the reference
    int reference;

    // in case there is a valid reference registered
    if((reference = luaScriptEngine->getObjectReference(value))) {
        // retrieves the value from the registry index
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, reference);

        // returns false
        return false;
    }
    else {
        // creates a new table for the reference
        lua_newtable(luaState);

        // retrieves the reference index
        reference = luaL_ref(luaState, LUA_REGISTRYINDEX);
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, reference);

        // sets the association between the c++ reference and the lua index
        luaScriptEngine->setObjectReference(value, reference);

        // returns true
        return true;
    }
}

int LuaMapping::getEngine(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargs(luaState, 0);

    // retrieves the lua script engine
    LuaScriptEngine *luaScriptEngine = LuaMapping::getScriptEngine(luaState);

    // creates a new table (the engine object)
    lua_newtable(luaState);
    lua_setself(luaState, luaScriptEngine->getEngine());
    lua_setnamefunction(luaState, "get_render", LuaEngine::getRender);

    // returns the number of return values
    return 1;
}

/**
 * Retrieves the lua script engine for the current lua interpreter.
 *
 * @param luaState The current lua state reference.
 * @return The lua script engine for the current lua interpreter.
 */
LuaScriptEngine *LuaMapping::getScriptEngine(lua_State *luaState) {
    // loads the global variable
    lua_getglobal(luaState, LUA_SCRIPT_ENGINE_GLOBAL_VARIABLE);

    // retrieves the lua script engine
    LuaScriptEngine *luaScriptEngine = (LuaScriptEngine *)  lua_tointeger(luaState, -1);

    // pops the engine global variable from stack
    lua_pop(luaState, 1);

    // returns the lua script engine
    return luaScriptEngine;
}

/**
 * Generates the constructors for all the classes in lua.
 * The constructors are registered in the mariachi global namespace.
 *
 * @param luaState The current lua state reference.
 */
void LuaMapping::generateConstructors(lua_State *luaState) {
    // in case the type information map is empty
    if(lua_typeinformationmap.size() == 0) {
        // constructs the type information map
        LuaMapping::constructTypeInformationMap();
    }

    // retrieves the lua type information map iterator
    std::map<std::string, LuaTypeInformation_t>::iterator lua_typeinformationmapIterator = lua_typeinformationmap.begin();

    // loads the engine base global variable
    lua_getglobal(luaState, LUA_SCRIPT_ENGINE_BASE_NAMESPACE);

    // iterates over all the lua type information map items
    while(lua_typeinformationmapIterator != lua_typeinformationmap.end()) {
        // retrieves the lua type name
        std::string luaTypeName = lua_typeinformationmapIterator->first;

        // retrieves the lua type information
        LuaTypeInformation_t &luaTypeInformation = lua_typeinformationmapIterator->second;

        // in case there is a valid lua constructor
        if(luaTypeInformation.luaConstructor) {
            // sets the lua constructor function in the map with the function lua type name
            lua_setnamefunction(luaState, luaTypeName.c_str(), luaTypeInformation.luaConstructor);
        }

        // increments the lua type information map iterator
        lua_typeinformationmapIterator++;
    }

    // pops the engine base global variable
    // from stack
    lua_pop(luaState, 1);
}

/**
 * Retrieves the int type value for the given type in
 * character value.
 *
 * @param charType The type in character value to retrieve the
 * int type.
 * @return The int type for the given character type.
 */
unsigned int LuaMapping::getIntType(const char *charType) {
    // in case the type information map is empty
    if(lua_typeinformationmap.size() == 0) {
        // constructs the type information map
         LuaMapping::constructTypeInformationMap();
    }

    return lua_typeinformationmap[charType].intType;
}

/**
 * Retrieves the constructor function for the given type in
 * character value.
 *
 * @param charType The type in character value to retrieve the
 * constructor.
 * @return The constructor for the given character type.
 */
LuaConstructor_t LuaMapping::getConstructor(const char *charType) {
    // in case the type information map is empty
    if(lua_typeinformationmap.size() == 0) {
        // constructs the type information map
         LuaMapping::constructTypeInformationMap();
    }

    // returns the contructor for the char type
    return lua_typeinformationmap[charType].constructor;
}

/**
 * Constructs the type information map for all the lua type values
 * the constructed map, includes information about the type.
 */
void LuaMapping::constructTypeInformationMap() {
    LuaTypeInformation_t listTypeInformation = { LUA_SCRIPT_ENGINE_LIST_TYPE_INT, NULL, NULL };
    LuaTypeInformation_t mapTypeInformation = { LUA_SCRIPT_ENGINE_MAP_TYPE_INT, NULL, NULL };
    LuaTypeInformation_t vectorTypeInformation = { LUA_SCRIPT_ENGINE_VECTOR_TYPE_INT, NULL, NULL };
    LuaTypeInformation_t nodeTypeInformation = { LUA_SCRIPT_ENGINE_NODE_TYPE_INT, (LuaConstructor_t) LuaNode::allocate, LuaNode::construct };
    LuaTypeInformation_t cubeNodeTypeInformation = { LUA_SCRIPT_ENGINE_CUBE_NODE_TYPE_INT, (LuaConstructor_t) LuaCubeNode::allocate, LuaCubeNode::construct };
    LuaTypeInformation_t sceneNodeTypeInformation = { LUA_SCRIPT_ENGINE_SCENE_NODE_TYPE_INT, (LuaConstructor_t) LuaSceneNode::allocate, LuaSceneNode::construct };

    lua_typeinformationmap["List"] = listTypeInformation;
    lua_typeinformationmap["Map"] = mapTypeInformation;
    lua_typeinformationmap["Vector"] = vectorTypeInformation;
    lua_typeinformationmap["Node"] = nodeTypeInformation;
    lua_typeinformationmap["CubeNode"] = cubeNodeTypeInformation;
    lua_typeinformationmap["SceneNode"] = sceneNodeTypeInformation;
}

bool LuaObject::allocate(lua_State *luaState, void *value) {
    bool return_value;

    // in case the reference is new
    if((return_value = LuaMapping::getReference(luaState, value))) {
        // sets the self value
        lua_setself(luaState, value);

        // sets the methods
        lua_setnamefunction(luaState, "cast_as", LuaObject::castAs);
        lua_setnamefunction(luaState, "type_id", LuaObject::typeId);
    }

    return return_value;
}

/**
 * Lua method to cast an object as a different object.
 * This method is a convenience to allow conversion and
 * type casting between types.
 *
 * @param luaState The current lua state reference.
 * @return The number of returning values.
 */
int LuaObject::castAs(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 1);

    // retrieves self
    void *self = (void *) lua_getself(luaState);

    // retrieves the cast as value
    const char *castAs = lua_tostring(luaState, -1);

    // retrieves the constructor for the cast value
    LuaConstructor_t constructor = LuaMapping::getConstructor(castAs);

    // calls the constructor for the cast value
    constructor(luaState, self);

    // returns the number of return values
    return 1;
}

/**
 * Lua method to retrieve the c++ type of the current object.
 * Use this method carefully as it is dangerous.
 *
 * @param luaState The current lua state reference.
 * @return The number of returning values.
 */
int LuaObject::typeId(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 0);

    // retrieves self
    void *self = (void *) lua_getself(luaState);

    // retrieves the type id value for self
    const char *typeIdValue = typeid(self).name();

    // pushes the type id value
    lua_pushfstring(luaState, typeIdValue);

    // returns the number of return values
    return 1;
}
