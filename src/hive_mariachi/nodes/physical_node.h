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

// __author__    = João Magalhães <joamag@hive.pt> & Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#include "cube_node.h"

#define PHYSICAL_NODE_DEFAULT_MASS 0.0f
#define DEFAULT_PHYSICAL_POSITION_ENABLED true
#define DEFAULT_PHYSICAL_ROTATION_ENABLED true

namespace mariachi {
    namespace nodes {
        class PhysicalNode : public CubeNode {
            private:
                //std::vector<RigElement_t *> rigList;
                float viscosity;
                float mass;
                structures::Coordinate3d_t inertia;
                std::list<float> *angularForces;
                std::list<float> *linearForces;
                structures::Coordinate3d_t linearVelocity;
                structures::Coordinate3d_t angularVelocity;
                bool physicalPositionEnabled;
                bool physicalRotationEnabled;

            public:
                PhysicalNode();
                PhysicalNode(const std::string &name);
                ~PhysicalNode();
                inline void initMass();
                inline void initTransforms();
                void addImpulse(const structures::Coordinate3d_t &impulse);
                void addTorque(const structures::Coordinate3d_t &torque);
                float getMass();
                void setMass(float mass);
                structures::Coordinate3d_t &getInertia();
                void setInertia(const structures::Coordinate3d_t &inertia);
                bool getPhysicalPositionEnabled();
                void setPhysicalPositionEnabled(bool physicalPositionEnabled);
                bool getPhysicalRotationEnabled();
                void setPhysicalRotationEnabled(bool physicalRotationEnabled);
                virtual inline unsigned int getNodeType() { return PHYSICAL_NODE_TYPE; };
        };
    }
}
