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

// __author__    = João Magalhães <joamag@hive.pt> & Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#ifdef MARIACHI_PLATFORM_IPHONE

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "../../main/engine.h"
#import "../../devices/input/multi_touch.h"
#import "renders/opengles_uikit_renderer.h"

#define DEFAULT_TOUCH_LIST_SIZE 1000

/**
 * Wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
 * The view content is basically an EAGL surface you render your OpenGL scene into.
 * Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
 */
@interface OpenglesUikitWindowView : UIView {
@private
    /**
     * The used opengles uikit renderer.
     */
    id <OpenglesUikitRenderer> renderer;

    /**
     * Flag to control animation.
     */
    BOOL animating;

    /**
     * Flag indicating the availability of the display link.
     */
    BOOL displayLinkSupported;

    /**
     * The interval in the animation frame (frame rate).
     */
    NSInteger animationFrameInterval;

    /**
     * Use of the CADisplayLink class is the preferred method for controlling your animation timing.
     * CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
     * The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
     * isn't available.
     */
    id displayLink;

    /**
     * Timer used to control animation whenever there's no display link available.
     */
    NSTimer *animationTimer;

    /**
     * The reference to the mariachi engine.
     */
    mariachi::Engine *_engine;
}

@property (readonly, nonatomic, getter = isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void) startAnimation;
- (void) stopAnimation;
- (void) drawView: (id) sender;
- (void) wrapTouches: (NSSet *) touches touchList: (mariachi::devices::Touch_t *) touchPositions;
- (mariachi::devices::TouchType_t) getTouchType: (UITouchPhase) touchPhase;
- (mariachi::Engine *) engine;
- (void) setEngine: (mariachi::Engine *) anEngine;

@end

#endif
