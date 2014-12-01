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

#import "stdafx.h"

#ifdef MARIACHI_PLATFORM_MACOSX

#import "opengl_cocoa_window_view.h"

#include <OpenGL/Glu.h>

using namespace mariachi;
using namespace mariachi::render_adapters;

@implementation OpenglCocoaWindowView

- (id) initWithFrame: (NSRect) frameRect {
    // creates a basic pixel format
    NSOpenGLPixelFormat *pixelFormat = [OpenglCocoaWindowView basicPixelFormat];

    // calls the super constructor with the defined pixel format
    self = [super initWithFrame:frameRect pixelFormat:pixelFormat];

    // register teh timer for drawing
    [self registerTimer];

    // register the callback for resize
    [self registerResize];

    // returns self
    return self;
}

- (void) drawRect: (NSRect) rect {
    // calls the display method in the opengl adapter
    openglAdapter->display();

    // flushes the open gl context buffer (to redraw the screen)
    [[self openGLContext] flushBuffer];
}

- (void) registerTimer {
    // starts animation timer for sixty frames per second
    NSTimer *timer = [NSTimer timerWithTimeInterval:(1.0f / 60.0f) target:self selector:@selector(animationTimer:) userInfo:nil repeats:YES];

    // registers the timer
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];

    // ensures timer fires during resize
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
}

- (void) registerResize {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(viewResized:) name:NSWindowDidResizeNotification object:[self window]];
}

- (void) animationTimer: (NSTimer *) timer {
    [self drawRect:[self bounds]];
}

/**
 * Called whenever the view is resized.
 *
 * @param notif The notification object.
 */
- (void) viewResized: (NSNotification *) notif {
    // retrieves the dimensions of the view
    NSRect dimensions = [self bounds];

    // calls the resize scene method in the opengl adapter
    openglAdapter->resizeScene(dimensions.size.width, dimensions.size.height);
}

- (void) initialize {
    openglAdapter->init();
}

- (void) display {
    // sets the display as dirty to refresh the draw
    [self setNeedsDisplay: YES];
}

- (Engine *) engine {
    return engine;
}

- (void) setEngine: (Engine *) anEngine {
    engine = anEngine;
}

- (OpenglAdapter *) openglAdapter {
    return openglAdapter;
}

- (void) setOpenglAdapter: (OpenglAdapter *) anOpenglAdapter {
    openglAdapter = anOpenglAdapter;
}

+ (NSOpenGLPixelFormat *) basicPixelFormat {
    NSOpenGLPixelFormatAttribute attributes[] = { NSOpenGLPFAWindow, NSOpenGLPFADoubleBuffer, NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) 16, (NSOpenGLPixelFormatAttribute) 0 };
    return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

@end

#endif
