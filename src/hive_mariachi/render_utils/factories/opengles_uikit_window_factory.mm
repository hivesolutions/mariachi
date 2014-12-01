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

#ifdef MARIACHI_PLATFORM_IPHONE

#import <UIKit/UIKit.h>

#import "../../render_adapters/opengles_adapter.h"
#import "../views/opengl_cocoa_window_view.h"
#import "opengles_uikit_window_factory.h"

using namespace mariachi;
using namespace mariachi::render_adapters;

NSAutoreleasePool *pool;
OpenglesAdapter *openglesAdapter;
Engine *engine;

void startOpenglesUikitWindow(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves the opengles adapter
    openglesAdapter = (OpenglesAdapter *) argumentsMap["adapter"];

    // retrieves the engine
    engine = (Engine *) argumentsMap["engine"];

    // creates an ns autorelease pool for garbage collection
    pool = [[NSAutoreleasePool alloc] init];
}

void stopOpenglesUikitWindow(void *arguments) {
}

void loopOpenglesUikitWindow(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves the arguments count
    int argc = (int) argumentsMap["argc"];

    // retrieves the arguments value
    char **argv = (char **) argumentsMap["argv"];

    // runs the application
    UIApplicationMain(argc, argv, nil, nil);

    // releases the application pool
    [pool release];
}

void updateOpenglesUikitWindow(void *arguments) {
}

#endif
