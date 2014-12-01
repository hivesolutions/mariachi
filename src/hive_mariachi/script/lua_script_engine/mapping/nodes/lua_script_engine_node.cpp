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

#include "lua_script_engine_node.h"

using namespace mariachi::nodes;
using namespace mariachi::script::lua;

bool LuaNode::allocate(lua_State *luaState, Node *value) {
    bool return_value;

    // in case the reference is new
    lua_mariachi_validate_reference(return_value, luaState, value, LUA_SCRIPT_ENGINE_NODE_TYPE, LuaObject::allocate) {
        // sets the methods
        lua_setnamefunction(luaState, "get_children_list", LuaNode::getChildrenList);

        // sets the type of the node
        lua_settype(luaState, LUA_SCRIPT_ENGINE_NODE_TYPE);
    }

    return return_value;
}

int LuaNode::construct(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargs(luaState, 0);

    // creates a new node
    Node *node = new Node();

    // creates and loads the node (in lua)
    LuaNode::allocate(luaState, node);

    // returns the number of return values
    return 1;
}

int LuaNode::getChildrenList(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 0);

    // retrieves self
    Node *self = (Node *) lua_getself(luaState);

    // retrieves the children list
    std::list<Node *> &childrenList = self->getChildrenList();

    // creates a new lua list object
    LuaListStructure<Node *> *luaList = new LuaListStructure<Node *>(&childrenList, (LuaConstructor_t) LuaNode::allocate);

    // creates and loads a list
    LuaList::allocate<Node *>(luaState, luaList);

    // returns the number of return values
    return 1;
}
