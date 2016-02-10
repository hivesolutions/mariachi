// Hive Mariachi Engine
// Copyright (c) 2008-2016 Hive Solutions Lda.
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
// __copyright__ = Copyright (c) 2008-2016 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

#pragma once

#ifdef MARIACHI_PLATFORM_WIN32
#define PID_TYPE DWORD
#define LOCAL_TIME(localTimeValue, timeValue) tm localTimeValueValue; localTimeValue = &localTimeValueValue; localtime_s(localTimeValue, timeValue)
#define SLEEP(miliseconds) Sleep(miliseconds)
#define GET_PID() GetCurrentProcessId()
#define SPRINTF(buffer, size, format, ...) sprintf_s(buffer, size, format, __VA_ARGS__)
#define GET_ENV(buffer, bufferSize, variableName) _dupenv_s(&buffer, &bufferSize, variableName)
#define FILE_EXISTS(filePath) GetFileAttributes(filePath) != 0xffffffff
#elif MARIACHI_PLATFORM_UNIX
#define PID_TYPE pid_t
#define LOCAL_TIME(localTimeValue, timeValue) localTimeValue = localtime(timeValue)
#define SLEEP(miliseconds) usleep((useconds_t) miliseconds * 1000)
#define GET_PID() getpid()
#define SPRINTF(buffer, size, format, ...) sprintf(buffer, format, __VA_ARGS__)
#define GET_ENV(buffer, bufferSize, variableName) buffer = getenv(variableName)
#define FILE_EXISTS(filePath) access(filePath, F_OK) == 0
#endif

#define CLOCK() clock()
