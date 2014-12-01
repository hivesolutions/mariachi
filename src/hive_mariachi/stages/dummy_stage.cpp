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

#include "../structures/structures.h"
#include "../importers/importers.h"
#include "../devices/devices.h"
#include "../user_interface/user_interface.h"

#include "dummy_stage.h"

using namespace mariachi;
using namespace mariachi::ui;
using namespace mariachi::nodes;
using namespace mariachi::stages;
using namespace mariachi::devices;
using namespace mariachi::importers;
using namespace mariachi::structures;

DummyStage::DummyStage() : Stage() {
}

DummyStage::DummyStage(Engine *engine) : Stage(engine) {
}

DummyStage::DummyStage(Engine *engine, const std::string &name) : Stage(engine, name) {
}

DummyStage::~DummyStage() {
}

void DummyStage::start(void *arguments) {
    Stage::start(arguments);
}

void DummyStage::stop(void *arguments) {
    Stage::stop(arguments);
}

void DummyStage::update(void *arguments) {
    Stage::update(arguments);
}
