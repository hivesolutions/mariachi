// Hive Mariachi Engine
// Copyright (c) 2008-2017 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Mariachi Engine. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2017 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#include "platforms.h"

/**
 * The name of the environment variable used to build
 * the mariachi path.
 */
#define HIVE_MARIACHI_ENVIRONMENT_PATH "MARIACHI_PATH"

/**
 * The root path being used.
 */
#define HIVE_MARIACHI_ROOT_PATH "."

#ifdef MARIACHI_PLATFORM_WIN32
#define MARIACHI_ENVIRONMENT_SEPARATOR ";"
#elif MARIACHI_PLATFORM_UNIX
#define MARIACHI_ENVIRONMENT_SEPARATOR ":"
#endif

#ifdef MARIACHI_PLATFORM_WIN32
#define HIVE_MARIACHI_BASE_PATH "/"
#elif MARIACHI_PLATFORM_IPHONE
#define HIVE_MARIACHI_BASE_PATH getBundleDirectory()
#elif MARIACHI_PLATFORM_UNIX
#define HIVE_MARIACHI_BASE_PATH "/"
#endif
