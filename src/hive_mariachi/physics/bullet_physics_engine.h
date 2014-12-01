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

#include "../../../lib/libbullet/src/btBulletDynamicsCommon.h"
#include "../nodes/physical_node.h"
#include "../structures/collision.h"

#include "physics_engine.h"

#define BULLET_DEFAULT_PHYSICS_RATE 1.0f / 15.0f

#define BULLET_DEFAULT_MAXIMUM_SUB_STEPS 4

namespace mariachi {
    namespace physics {
        class BulletPhysicsEngine : public PhysicsEngine {
            private:
                /**
                 * The bullet engine collision configuration.
                 */
                btDefaultCollisionConfiguration *collisionConfiguration;

                /**
                 * The bullet engine dispatcher.
                 */
                btCollisionDispatcher *dispatcher;

                /**
                 * The bullet engine broad phase.
                 */
                btBroadphaseInterface *broadPhase;

                /**
                 * The bullet engine impulse constraint solver.
                 */
                btSequentialImpulseConstraintSolver *solver;

                /**
                 * The bullet engine world representation object.
                 */
                btDiscreteDynamicsWorld *dynamicsWorld;

                /**
                 * The map associating the physical nodes with the bullet engine
                 * rigid body structures.
                 */
                std::map<nodes::PhysicalNode *, btRigidBody *> physicalNodeRigidBodyMap;

                /**
                 * The map associating the bullet engine rigid body structures with
                 * the physical nodes.
                 */
                std::map<btRigidBody *, nodes::PhysicalNode *> rigidBodyPhysicalNodeMap;

                /**
                 * The maximum sub steps allowed until full physics calculation, regulates interpolation in the bullet engine.
                 */
                int maximumSubSteps;

                btClock clock;
                int lastUpdateTimeMicroseconds;

                btRigidBody *getRigidBody(nodes::PhysicalNode *physicalNode, nodes::CollisionNode *collisionNode, void *arguments);
                void setRigidBodyCollisionFlags(btRigidBody *rigidBody, nodes::CollisionNode *collisionNode);

            public:
                BulletPhysicsEngine();
                BulletPhysicsEngine(Engine *engine);
                ~BulletPhysicsEngine();
                inline void initPhysicsRate();
                void load(void *arguments);
                void unload(void *arguments);
                void update(float delta, void *arguments);
                void update(float delta);
                void update();
                std::vector<structures::Collision3d_t> getCollisions(void *arguments);
                void registerPhysics(nodes::PhysicalNode *physicalNode, void *arguments);
                void registerCollision(nodes::CollisionNode *collisionNode, void *arguments);
                void unregisterCollision(nodes::CollisionNode *collisionNode, void *arguments);
                CubeSolid *createCubeSolid();
                SphereSolid *createSphereSolid();
                void updatePhysicalNodePosition(nodes::PhysicalNode *physicalNode, const structures::Coordinate3d_t &position);
                void addPhysicalNodeImpulse(nodes::PhysicalNode *physicalNode, const structures::Coordinate3d_t &impulse, const structures::Coordinate3d_t &relativePosition);
                void setPhysicalNodeVelocity(nodes::PhysicalNode *physicalNode, const structures::Coordinate3d_t &velocity);
                void setGravity(const structures::Coordinate3d_t &gravity);
        };
    }
}
