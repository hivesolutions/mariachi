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

#ifdef MARIACHI_PLATFORM_MACOSX

#import <Cocoa/Cocoa.h>

#import "../../render_adapters/opengl_adapter.h"
#import "../views/opengl_cocoa_window_view.h"
#import "opengl_cocoa_window_factory.h"

using namespace mariachi;
using namespace mariachi::render_adapters;

NSAutoreleasePool *pool;
OpenglCocoaWindowView *openGlCocoaView;

void startOpenglCocoaWindow(void *arguments) {
    // retrieves the arguments map from the arguments
    std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

    // retrieves the opengl adapter
    OpenglAdapter *openglAdapter = (OpenglAdapter *) argumentsMap["adapter"];

    // retrieves the engine
    Engine *engine = (Engine *) argumentsMap["engine"];

    // retrieves the shared application reference
    [NSApplication sharedApplication];

    // creates an ns autorelease pool for garbage collection
    pool = [[NSAutoreleasePool alloc] init];

    // creates the window style
    int style = NSClosableWindowMask | NSResizableWindowMask | NSTexturedBackgroundWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;

    // creates the ns window for the display
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(50, 50, 600, 400) styleMask:style backing:NSBackingStoreBuffered defer:NO];

    // creates the ns window for the display
    //NSWindow *window = [[NSWindow alloc] initWithContentRect:[[NSScreen mainScreen] frame] styleMask:style backing:NSBackingStoreBuffered defer:NO];

    // initializes the cocoa opengl window view
    openGlCocoaView = [[OpenglCocoaWindowView alloc] init];

    // sets the window to not have borders
    //[window setStyleMask:NSBorderlessWindowMask];

    // sets the window to be top level
    //int windowLevel = CGShieldingWindowLevel();
    //[window setLevel:windowLevel];

    // sets the opengl adapter in the opengl cocoa view
    [openGlCocoaView setOpenglAdapter:openglAdapter];

    // sets the engine in the opengl cocoa view
    [openGlCocoaView setEngine:engine];

    // sets the content view in the window
    [window setContentView:openGlCocoaView];

    // makes the window the key window and the front one
    [window makeKeyAndOrderFront:window];

    // calls the initialize method in the opengl cocoa view
    [openGlCocoaView initialize];

    // calls the view resized method in the opengl cocoa view
    [openGlCocoaView viewResized:nil];
}

void stopOpenglCocoaWindow(void *arguments) {
}

void loopOpenglCocoaWindow(void *arguments) {
    // runs the application
    [NSApp run];

    // releases the application pool
    [pool release];
}

void updateOpenglCocoaWindow(void *arguments) {
    // sets the opengl cocoa view as dirty to refresh the screen
    [openGlCocoaView setNeedsDisplay:YES];
}

#endif
