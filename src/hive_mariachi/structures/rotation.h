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

// __author__    = Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

namespace mariachi {
    namespace structures {
        /**
         * Represents a rotation in a 2d space.
         *
         * @param angle The angle value of the rotation.
         * @param x The x value of the rotation axis.
         * @param y The y value of the rotation axis.
         */
        typedef struct Rotation2d_t {
            float angle;
            float x;
            float y;
        } Rotation2d;

        /**
         * Represents a rotation in a 3d space.
         *
         * @param angle The angle value of the rotation.
         * @param x The x value of the rotation axis.
         * @param y The y value of the rotation axis.
         * @param z The z value of the rotation axis.
         */
        typedef struct Rotation3d_t {
            float angle;
            float x;
            float y;
            float z;
        } Rotation3d;
    }
}
