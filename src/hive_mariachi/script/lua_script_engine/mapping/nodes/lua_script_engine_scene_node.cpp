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

#include "lua_script_engine_scene_node.h"

using namespace mariachi::nodes;
using namespace mariachi::script::lua;

bool LuaSceneNode::allocate(lua_State *luaState, SceneNode *value) {
    bool return_value;

    // in case the reference is new
    lua_mariachi_validate_reference(return_value, luaState, value, LUA_SCRIPT_ENGINE_SCENE_NODE_TYPE, LuaCubeNode::allocate) {
        // sets the type of the node
        lua_settype(luaState, LUA_SCRIPT_ENGINE_NODE_TYPE);
    }

    return return_value;
}

int LuaSceneNode::construct(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargs(luaState, 0);

    // creates a new scene node
    SceneNode *sceneNode = new SceneNode();

    // creates and loads the scene node (in lua)
    LuaNode::allocate(luaState, sceneNode);

    // returns the number of return values
    return 1;
}
