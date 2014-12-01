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

#define ADD_TO_STAGES_LIST(stage) stagesList->push_back(stage);

#ifdef MARIACHI_STAGE_RENDER
ADD_TO_STAGES_LIST(new RenderStage(this, std::string("render")));
#endif

#ifdef MARIACHI_STAGE_CAMERA
ADD_TO_STAGES_LIST(new CameraStage(this, std::string("camera")));
#endif

#ifdef MARIACHI_STAGE_CONSOLE
ADD_TO_STAGES_LIST(new ConsoleStage(this, std::string("console")));
#endif

#ifdef MARIACHI_STAGE_DUMMY
ADD_TO_STAGES_LIST(new DummyStage(this, std::string("dummy")));
#endif
