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

#pragma once

namespace mariachi {
    namespace devices {
        typedef enum DeviceState_t {
            UP = 1,
            DOWN,
            READY,
            NOT_READY
        } DeviceState;

        class Device {
            private:
                DeviceState state;

            public:
                Device();
                ~Device();
                virtual void start(void *arguments) { this->state = UP; };
                virtual void stop(void *arguments) { this->state = DOWN; };
                virtual DeviceState getState() { return this->state; };
                virtual void setState(DeviceState statestate) { this->state = state; };
        };
    }
}
