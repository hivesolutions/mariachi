-- Hive Mariachi Engine
-- Copyright (C) 2008 Hive Solutions Lda.
--
-- This file is part of Hive Mariachi Engine.
--
-- Hive Mariachi Engine is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- Hive Mariachi Engine is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with Hive Mariachi Engine. If not, see <http://www.gnu.org/licenses/>.

-- __author__    = João Magalhães <joamag@hive.pt>
-- __version__   = 1.0.0
-- __revision__  = $LastChangedRevision$
-- __date__      = $LastChangedDate$
-- __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
-- __license__   = GNU General Public License (GPL), Version 3

local mariachi = { funcs = {} }
mariachi.__index = mariachi

function mariachi.__call(_, value)
    return mariachi:new(value)
end

function mariachi:new(value, chained)
    return setmetatable({ _val = value, chained = chained or false }, self)
end

function mariachi.dir(_, value)
    table.foreach(value, print)
end

function mariachi.functions()
    return mariachi.keys(mariachi.funcs)
end

-- add aliases
mariachi.methods = mariachi.functions

local function wrap_functions_for_oo_support()
    local function value_and_chained(value_or_self)
        local chained = false
        if getmetatable(value_or_self) == mariachi then
            chained = value_or_self.chained
            value_or_self = value_or_self._val
        end
        return value_or_self, chained
    end

    local function value_or_wrap(value, chained)
        if chained then value = mariachi:new(value, true) end
        return value
    end

    for fn, func in pairs(mariachi.funcs) do
        mariachi[fn] = function(obj_or_self, ...)
            local obj, chained = value_and_chained(obj_or_self)
            return value_or_wrap(func(obj, ...), chained)
        end
    end
end

wrap_functions_for_oo_support()

return mariachi:new()
