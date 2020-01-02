// Hive Mariachi Engine
// Copyright (c) 2008-2020 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "../lua_script_engine_mapping.h"

#define LUA_SCRIPT_ENGINE_LIST_TYPE "_t_LIST"

namespace mariachi {
    namespace script {
        namespace lua {
            /**
             * Structural class used to be passed as argument to create
             * a new list in lua.
             */
            template<typename T> class LuaListStructure {
                public:
                    /**
                     * The stl list value to be encapsulated.
                     */
                    std::list<T> *listValue;

                    /**
                     * The constructor function to be used to construct the list elements.
                     */
                    bool (*constructor)(lua_State *luaState, void *value);

                    /**
                     * Constructor of the class.
                     */
                    LuaListStructure() { };

                    /**
                     * Constructor of the class.
                     *
                     * @param listValue The stl list value to be encapsulated.
                     * @param constructor The constructor function to be used to construct
                     * the list elements.
                     */
                    LuaListStructure(std::list<T> *listValue, LuaConstructor_t constructor) {
                        this->listValue = listValue;
                        this->constructor = constructor;
                    }

                    /**
                     * The destructor of the class.
                     */
                    ~LuaListStructure() { };
            };

            class LuaList {
                private:

                public:
                    template<class T> static bool allocate(lua_State *luaState, LuaListStructure<T> *value) {
                        bool return_value;

                        // in case the reference is new
                        lua_mariachi_validate_reference(return_value, luaState, value, LUA_SCRIPT_ENGINE_LIST_TYPE, LuaObject::allocate) {
                            // sets the type of the node
                            lua_settype(luaState, LUA_SCRIPT_ENGINE_LIST_TYPE);

                            // sets the methods
                            lua_setnamefunction(luaState, "front", LuaList::front<T>);
                            lua_setnamefunction(luaState, "get", LuaList::get<T>);
                        }

                        return return_value;
                    }

                    template<typename T> static int front(lua_State *luaState) {
                        // validates the number of arguments
                        lua_assertargsmethod(luaState, 0);

                        // retrieves self
                        LuaListStructure<T> *self = (LuaListStructure<T> *) lua_getself(luaState);

                        // retrieves the front value
                        T frontValue = self->listValue->front();

                        // constructs the value
                        self->constructor(luaState, frontValue);

                        // returns the number of return values
                        return 1;
                    }

                    template<typename T> static int get(lua_State *luaState) {
                        // validates the number of arguments
                        lua_assertargsmethod(luaState, 1);

                        // retrieves self
                        LuaListStructure<T> *self = (LuaListStructure<T> *) lua_getself(luaState);

                        // retrieves the index
                        unsigned int index = (int) lua_tointeger(luaState, -1);

                        // retrieves the list value size
                        unsigned int listValueSize = self->listValue->size();

                        // in case the index is out of bounds
                        if(index >= listValueSize || listValueSize <= 0) {
                            // returns no arguments
                            return 0;
                        }

                        // retrieves the list value iterator
                        typename std::list<T>::iterator listValueIterator = self->listValue->begin();

                        // allocates space for the current list value
                        T currentListValue;

                        // iterates over all the values in the list
                        for(unsigned int _index = 0; _index < index + 1 && listValueIterator != self->listValue->end(); _index++) {
                            // retrieves the current list value
                            currentListValue = *listValueIterator;

                            // increments the list value iterator
                            listValueIterator++;
                        }

                        // constructs the value
                        self->constructor(luaState, currentListValue);

                        // returns the number of return values
                        return 1;
                    }
            };
        }
    }
}
