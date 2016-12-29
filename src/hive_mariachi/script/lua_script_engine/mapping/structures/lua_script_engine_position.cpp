// Hive Mariachi Engine
// Copyright (c) 2008-2017 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2017 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#include "stdafx.h"

#include "lua_script_engine_position.h"

using namespace mariachi::structures;
using namespace mariachi::script::lua;

bool LuaCoordinate3d_t::allocate(lua_State *luaState, Coordinate3d_t *value) {
    bool return_value;

    if((return_value = LuaMapping::getReference(luaState, value))) {
    }

    lua_setnamefloat(luaState, "x", value->x);
    lua_setnamefloat(luaState, "y", value->y);
    lua_setnamefloat(luaState, "z", value->z);

    return return_value;
}

int LuaCoordinate3d_t::construct(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargs(luaState, 0);

    // creates a new coordinate 3d
    Coordinate3d_t *coordinate3d = new Coordinate3d_t();

    // creates and loads the coordinate 3d (in lua)
    LuaCoordinate3d_t::allocate(luaState, coordinate3d);

    // returns the number of return values
    return 1;
}
