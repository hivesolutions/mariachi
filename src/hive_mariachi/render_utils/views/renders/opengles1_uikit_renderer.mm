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

#ifdef MARIACHI_PLATFORM_IPHONE

#import "../../../render_adapters/opengles_adapter.h"
#import "opengles1_uikit_renderer.h"

using namespace mariachi::render_adapters;

extern OpenglesAdapter *openglesAdapter;

@implementation Opengles1UikitRenderer

- (id) initWithLayer: (CAEAGLLayer *) layer depthBuffer: (BOOL) aDepthBuffer {
    if (self = [super init]) {
        // starts the opengles subsystem
        [self startOpengles:layer depthBuffer:aDepthBuffer];

        // initializes the opengles adapter
        openglesAdapter->init();
    }

    return self;
}

- (void) startOpengles: (CAEAGLLayer *) layer depthBuffer: (BOOL) aDepthBuffer {
    // creates a new opengles context
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

    // in case the creation of the context was not successful
    if (!context || ![EAGLContext setCurrentContext:context]) {
        // releases the object
        [self release];

        // return invalid
        return;
    }

    // creates default framebuffer object, the backing will be
    // allocated for the current layer in resizeFromLayer
    glGenFramebuffersOES(1, &defaultFramebuffer);
    glGenRenderbuffersOES(1, &colorRenderbuffer);

    // binds the framebuffer
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);

    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);

    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

    // in case the screen has a depth buffer
    if(aDepthBuffer) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);

        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }

    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return;
    }
}

- (void) render {
    // sets the current context
    [EAGLContext setCurrentContext:context];

    // binds the framebuffer
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);

    // calls the display method in the opengles adapter
    openglesAdapter->display();

    // binds the render buffer
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);

    // displays (draws) the render buffer in the screen
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (BOOL) resizeFromLayer: (CAEAGLLayer *) layer {
    // allocates color buffer backing based on the current layer size
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];

    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }

    // resizes the scene
    openglesAdapter->resizeScene(backingWidth, backingHeight);

    return YES;
}

- (void) dealloc {
    // tear down open gl
    if (defaultFramebuffer) {
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }

    if (colorRenderbuffer) {
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }

    // tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];

    [context release];
    context = nil;

    [super dealloc];
}

@end


#endif
