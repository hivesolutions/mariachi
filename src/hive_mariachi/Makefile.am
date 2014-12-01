# Hive Mariachi Engine
# Copyright (C) 2008 Hive Solutions Lda.
#
# This file is part of Hive Mariachi Engine.
#
# Hive Mariachi Engine is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Hive Mariachi Engine is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Hive Mariachi Engine. If not, see <http:#www.gnu.org/licenses/>.

# __author__    = João Magalhães <joamag@hive.pt>
# __version__   = 1.0.0
# __revision__  = $LastChangedRevision: 2390 $
# __date__      = $LastChangedDate: 2009-04-02 08:36:50 +0100 (qui, 02 Abr 2009) $
# __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
# __license__   = GNU General Public License (GPL), Version 3

include $(top_srcdir)/Common.am

lib_LIBRARIES = libmariachi.a

libmariachi_a_CFLAGS = $(AM_CFLAGS)
libmariachi_a_CXXFLAGS = $(AM_CXXFLAGS)

lua_sources = script/lua_script_engine.cpp \
script/lua_script_engine/mapping/base/lua_script_engine_list.cpp \
script/lua_script_engine/mapping/base/lua_script_engine_map.cpp \
script/lua_script_engine/mapping/base/lua_script_engine_vector.cpp \
script/lua_script_engine/mapping/lua_script_engine_mapping.cpp \
script/lua_script_engine/mapping/main/lua_script_engine_engine.cpp \
script/lua_script_engine/mapping/nodes/lua_script_engine_cube_node.cpp \
script/lua_script_engine/mapping/nodes/lua_script_engine_node.cpp \
script/lua_script_engine/mapping/nodes/lua_script_engine_scene_node.cpp \
script/lua_script_engine/mapping/structures/lua_script_engine_position.cpp \
script/lua_script_engine/util/lua_script_engine_util.cpp

libmariachi_a_SOURCES = stdafx.cpp \
algorithms/compression/huffman.cpp \
algorithms/hashing/crc32.cpp \
algorithms/hashing/hash_function.cpp \
algorithms/hashing/md5.cpp \
algorithms/path_finding/a_star.cpp \
algorithms/path_finding/path_finder.cpp \
configuration/configuration_list.cpp \
configuration/configuration_manager.cpp \
configuration/configuration_map.cpp \
configuration/configuration_structure.cpp \
configuration/parsers/configuration_parser.cpp \
configuration/parsers/json_configuration_parser.cpp \
console/console_manager.cpp \
debugging/debug_engine.cpp \
debugging/devices/debug_device.cpp \
debugging/devices/network_keyboard.cpp \
devices/device.cpp \
devices/input/gamepad.cpp \
devices/input/input_device.cpp \
devices/input/keyboard.cpp \
devices/input/mouse.cpp \
devices/input_output/input_output_device.cpp \
devices/input_output/network/datagram_network.cpp \
devices/input_output/network/network_device.cpp \
devices/input_output/network/packet_network.cpp \
devices/output/output_device.cpp \
exceptions/exception.cpp \
exceptions/runtime_exception.cpp \
importers/bmp_loader.cpp \
importers/importer.cpp \
importers/md2_importer.cpp \
importers/model_importer.cpp \
importers/texture_importer.cpp \
logging/logger.cpp \
main/engine.cpp \
main/module.cpp \
nodes/actor_node.cpp \
nodes/camera_node.cpp \
nodes/collision_node.cpp \
nodes/cube_node.cpp \
nodes/lens_node.cpp \
nodes/model_node.cpp \
nodes/node.cpp \
nodes/physical_node.cpp \
nodes/scene2d_node.cpp \
nodes/scene_node.cpp \
nodes/square_node.cpp \
patterns/observable.cpp \
physics/bullet_physics_engine.cpp \
physics/bullet_physics_engine/collision/bullet_physics_engine_collision_solid.cpp \
physics/bullet_physics_engine/collision/bullet_physics_engine_cube_solid.cpp \
physics/bullet_physics_engine/collision/bullet_physics_engine_sphere_solid.cpp \
physics/bullet_physics_engine/physical_node_motion_state.cpp \
physics/collision/collision_solid.cpp \
physics/collision/cube_solid.cpp \
physics/collision/sphere_solid.cpp \
physics/constraints/cone_twist_constraint.cpp \
physics/constraints/constraint.cpp \
physics/constraints/generic_six_dof.cpp \
physics/constraints/hinge_constraint.cpp \
physics/constraints/point_to_point_constraint.cpp \
physics/constraints/slider_constraint.cpp \
physics/physics_engine.cpp \
render/render_information.cpp \
render_adapters/direct3d9_adapter.cpp \
render_adapters/direct3d_adapter.cpp \
render_adapters/opengl_adapter.cpp \
render_adapters/opengles1_adapter.cpp \
render_adapters/opengles_adapter.cpp \
render_adapters/render_adapter.cpp \
render_utils/direct3d_win32_window.cpp \
render_utils/direct3d_window.cpp \
render_utils/factories/opengl_cocoa_window_factory.mm \
render_utils/factories/opengles_uikit_window_factory.mm \
render_utils/opengl_cocoa_window.cpp \
render_utils/opengl_glut_window.cpp \
render_utils/opengl_win32_window.cpp \
render_utils/opengl_window.cpp \
render_utils/opengles_uikit_window.cpp \
render_utils/opengles_window.cpp \
render_utils/views/opengl_cocoa_window_view.mm \
render_utils/views/opengles_uikit_window_view.mm \
render_utils/views/renders/opengles1_uikit_renderer.mm \
script/script_engine.cpp \
serialization/json_reader.cpp \
serialization/json_value.cpp \
serialization/json_writer.cpp \
stages/console_stage.cpp \
stages/dummy_stage.cpp \
stages/render_stage.cpp \
stages/stage.cpp \
stages/stage_runner.cpp \
structures/oct_tree.cpp \
structures/oct_tree_node.cpp \
structures/texture.cpp \
tasks/function_caller_task.cpp \
tasks/task.cpp \
user_interface/layout/base_layout.cpp \
user_interface/layout/flow_layout.cpp \
user_interface/layout/grid_bag_layout.cpp \
user_interface/ui_box_component_node.cpp \
user_interface/ui_button_node.cpp \
user_interface/ui_component_node.cpp \
user_interface/ui_container_node.cpp \
user_interface/ui_node.cpp \
user_interface/ui_panel_node.cpp \
user_interface/ui_view_port_node.cpp \
util/bit_util.cpp \
util/box_util.cpp \
util/byte_util.cpp \
util/cpu_util.cpp \
util/geometry_util.cpp \
util/string_util.cpp \
util/vector_util.cpp 

if LINK_LUA
    libmariachi_a_SOURCES += $(lua_sources)
endif

