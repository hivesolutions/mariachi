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

-- prints the current running version
io.write("Running ", _VERSION, "\n")

-- creates a new cube node
new_cube_node = mariachi.CubeNode()

-- retrieves the engine
engine = mariachi.get_engine()

-- retrieves the engine
engine = mariachi.get_engine()

-- retrieves the render node
render = engine:get_render()

-- retrieves the children list
children_list = render:get_children_list()

-- retrieves the front node
front_node = children_list:front()

front_node_scene = front_node:cast_as("SceneNode")

-- sets a new position
position = render:set_position(1.1, 2.0, 3.0)
position = render:get_position()

-- prints the positions
print(position.x)
print(position.y)
print(position.z)

io.write("engine_get_render() returned ", position.x, ", ", position.y, ", ", position.z, ", ", "\n")
