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

#include "lua_script_engine_cube_node.h"

using namespace mariachi::nodes;
using namespace mariachi::structures;
using namespace mariachi::script::lua;

bool LuaCubeNode::allocate(lua_State *luaState, CubeNode *value) {
    bool return_value;

    // in case the reference is new
    lua_mariachi_validate_reference(return_value, luaState, value, LUA_SCRIPT_ENGINE_CUBE_NODE_TYPE, LuaNode::allocate) {
        // sets the methods
        lua_setnamefunction(luaState, "get_position", LuaCubeNode::getPosition);
        lua_setnamefunction(luaState, "set_position", LuaCubeNode::setPosition);

        // sets the type of the node
        lua_settype(luaState, LUA_SCRIPT_ENGINE_CUBE_NODE_TYPE);
    }

    return return_value;
}

int LuaCubeNode::construct(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargs(luaState, 0);

    // creates a new cube node
    CubeNode *cubeNode = new CubeNode();

    // creates and loads the cube node (in lua)
    LuaCubeNode::allocate(luaState, cubeNode);

    // returns the number of return values
    return 1;
}

int LuaCubeNode::getPosition(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 0);

    // retrieves self
    CubeNode *self = (CubeNode *) lua_getself(luaState);

    // retrieves the position
    Coordinate3d_t position = self->getPosition();

    // creates and loads a coordinate 3d
    LuaCoordinate3d_t::allocate(luaState, &position);

    // returns the number of return values
    return 1;
}

int LuaCubeNode::setPosition(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 3);

    // retrieves self
    CubeNode *self = (CubeNode *) lua_getself(luaState);

    // retrieves the coordinates
    float x = (float) lua_tonumber(luaState, 1);
    float y = (float) lua_tonumber(luaState, 2);
    float z = (float) lua_tonumber(luaState, 3);

    // sets the position
    self->setPosition(x, y, z);

    // returns the number of return values
    return 0;
}
