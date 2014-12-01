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

#include "stdafx.h"

#include "bullet_physics_engine/_bullet_physics_engine.h"

#include "bullet_physics_engine.h"

using namespace mariachi;
using namespace mariachi::nodes;
using namespace mariachi::physics;
using namespace mariachi::structures;

BulletPhysicsEngine::BulletPhysicsEngine() : PhysicsEngine() {
    this->initPhysicsRate();

    this->clock = btClock();
    this->lastUpdateTimeMicroseconds = this->clock.getTimeMicroseconds();
}

BulletPhysicsEngine::BulletPhysicsEngine(Engine *engine) : PhysicsEngine(engine) {
    this->initPhysicsRate();

    this->clock = btClock();
    this->lastUpdateTimeMicroseconds = this->clock.getTimeMicroseconds();
}

BulletPhysicsEngine::~BulletPhysicsEngine() {
}

inline void BulletPhysicsEngine::initPhysicsRate() {
    // initializes the physics rate
    this->physicsRate = BULLET_DEFAULT_PHYSICS_RATE;

    // initializes the underlying sub steps parameter
    this->maximumSubSteps = BULLET_DEFAULT_MAXIMUM_SUB_STEPS;
}

void BulletPhysicsEngine::load(void *arguments) {
    // creates the collision configuration (contains default setup for memory collision setup)
    this->collisionConfiguration = new btDefaultCollisionConfiguration();

    // creates the default collision dispatcher
    this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // creates a broad phase as a general purpose broadphase
    this->broadPhase = new btDbvtBroadphase();

    // creates a default constraint solver
    this->solver = new btSequentialImpulseConstraintSolver;

    // creates the dynamics world
    this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
}

void BulletPhysicsEngine::unload(void *arguments) {
    // retrieves the physical node rigid body map iterator
    std::map<PhysicalNode *, btRigidBody *>::iterator physicalNodeRigidBodyMapIterator = this->physicalNodeRigidBodyMap.begin();

    // iterates over all the current rigid bodies
    while(physicalNodeRigidBodyMapIterator != this->physicalNodeRigidBodyMap.end()) {
        // retrieves the current rigid body
        btRigidBody *rigidBody = physicalNodeRigidBodyMapIterator->second;

        // removes the rigid body from the dynamics world
        this->dynamicsWorld->removeRigidBody(rigidBody);

        // deletes the rigid body motion state
        delete rigidBody->getMotionState();

        // deletes the rigid body
        delete rigidBody;

        // increments the physical node rigid body map iterator
        physicalNodeRigidBodyMapIterator++;
    }

    // clears the physical node rigid body map
    this->physicalNodeRigidBodyMap.clear();

    // deletes the dynamics world
    delete this->dynamicsWorld;

    // deletes the solver
    delete this->solver;

    // deletes the collision configuration
    delete this->collisionConfiguration;

    // deletes the dispatcher
    delete this->dispatcher;

    // deletes the borad phase
    delete this->broadPhase;
}

void BulletPhysicsEngine::update(float delta, void *arguments) {
    int maximumSubSteps = this->maximumSubSteps;

    // in case the arguments are valid
    if(arguments) {
        // retrieves the arguments map from the arguments
        std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

        // retrieves the maximum sub steps
        maximumSubSteps = (long long) argumentsMap["maximum_sub_steps"];
    }

    // runs a simulation step
    this->dynamicsWorld->stepSimulation(delta, this->maximumSubSteps, this->physicsRate);
}

void BulletPhysicsEngine::update(float delta) {
    // runs a simulation step
    this->dynamicsWorld->stepSimulation(delta, this->maximumSubSteps, this->physicsRate);
}

void BulletPhysicsEngine::update() {
    // retrieves the current time
    unsigned int currentTimeMicroseconds = this->clock.getTimeMicroseconds();

    // computes the elapsed time
    unsigned int elapsedTimeMicroseconds = currentTimeMicroseconds - this->lastUpdateTimeMicroseconds;

    // computes the elapsed time in seconds
    float elapsedTimeSeconds = elapsedTimeMicroseconds / 1000000.0f;

    // runs a simulation step
    this->dynamicsWorld->stepSimulation(elapsedTimeSeconds, this->maximumSubSteps, this->physicsRate);

    // updates the last update time
    this->lastUpdateTimeMicroseconds = this->clock.getTimeMicroseconds();
}

std::vector<Collision3d_t> BulletPhysicsEngine::getCollisions(void *arguments) {
    // the collision list
    std::vector<Collision3d_t> collisions;

    // retrieves the number of manifolds
    int numberManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();

    // iterates over all the contact manifolds creating the corresponding collision
    for(int index = 0; index < numberManifolds; index++) {
        // the current collision
        Collision3d_t collision;

        // retrieves the current manifold
        btPersistentManifold *contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(index);

        // retrieves the collision's rigid bodies
        btRigidBody *firstRigidBody = (btRigidBody *) contactManifold->getBody0();
        btRigidBody *secondRigidBody = (btRigidBody *) contactManifold->getBody1();

        // retrieves the motion state's physical nodes into the collision
        collision.firstPhysicalNode = this->rigidBodyPhysicalNodeMap[firstRigidBody];
        collision.secondPhysicalNode = this->rigidBodyPhysicalNodeMap[secondRigidBody];

        // retrieves the number of contacts
        int numberContacts = contactManifold->getNumContacts();

        // interates over all the contacts
        for(int _index = 0; _index < numberContacts && _index < COLLISION_POINT_LIST_SIZE; _index++) {
            // retrieves the current contact point
            btManifoldPoint &contactPoint = contactManifold->getContactPoint(_index);

            // in case the contact point distance is valid
            if(contactPoint.getDistance() < 0.0f) {
                // the collision point
                CollisionPoint3d_t collisionPoint;

                // retrieves the contact point in both objects
                const btVector3 &contactPointFirstRigidBody = contactPoint.getPositionWorldOnA();
                const btVector3 &contactPointSecondRigidBody = contactPoint.getPositionWorldOnB();

                // retrieves the normal vectors on the second object
                const btVector3 &contactPointNormalSecondRigidBody = contactPoint.m_normalWorldOnB;

                // sets the position on the first object
                collisionPoint.positionFirstPhysicalNode.x = contactPointFirstRigidBody.x();
                collisionPoint.positionFirstPhysicalNode.y = contactPointFirstRigidBody.y();
                collisionPoint.positionFirstPhysicalNode.z = contactPointFirstRigidBody.z();

                // sets the position on the second object
                collisionPoint.positionSecondPhysicalNode.x = contactPointSecondRigidBody.x();
                collisionPoint.positionSecondPhysicalNode.y = contactPointSecondRigidBody.y();
                collisionPoint.positionSecondPhysicalNode.z = contactPointSecondRigidBody.z();

                // sets the normal on the second object
                collisionPoint.normalSecondPhysicalNode.x = contactPointNormalSecondRigidBody.x();
                collisionPoint.normalSecondPhysicalNode.y = contactPointNormalSecondRigidBody.y();
                collisionPoint.normalSecondPhysicalNode.z = contactPointNormalSecondRigidBody.z();

                // sets the point in the collision's point list
                collision.collisionPointList[_index] = collisionPoint;
            }
        }

        // adds the collision to the collision list
        collisions.push_back(collision);
    }

    return collisions;
}

void BulletPhysicsEngine::registerPhysics(PhysicalNode *physicalNode, void *arguments) {
    // retrieves the rigid body
    this->getRigidBody(physicalNode, NULL, arguments);
}

void BulletPhysicsEngine::registerCollision(CollisionNode *collisionNode, void *arguments) {
    // retrieves the physical node
    PhysicalNode *physicalNode = (PhysicalNode *) collisionNode->getParent();

    // retrieves the rigid body
    this->getRigidBody(physicalNode, collisionNode, arguments);
}

void BulletPhysicsEngine::unregisterCollision(CollisionNode *collisionNode, void *arguments) {
    // retrieves the physical node
    PhysicalNode *physicalNode = (PhysicalNode *) collisionNode->getParent();

    // retrieves the rigid body
    btRigidBody *physicalNodeRigidBody = this->physicalNodeRigidBodyMap[physicalNode];

    // removes the rigid body from the world
    this->dynamicsWorld->removeRigidBody(physicalNodeRigidBody);
}

CubeSolid *BulletPhysicsEngine::createCubeSolid() {
    return new BulletPhysicsEngineCubeSolid();
}

SphereSolid *BulletPhysicsEngine::createSphereSolid() {
    return new BulletPhysicsEngineSphereSolid();
}

btRigidBody *BulletPhysicsEngine::getRigidBody(PhysicalNode *physicalNode, CollisionNode *collisionNode, void *arguments) {
    // the collision filter group flags
    short collisionFilterGroup = NULL;

    // the collision filter mask flags
    short collisionFilterMask = NULL;

    // in case there are valid arguments available
    if(arguments != NULL) {
        // retrieves the arguments map from the arguments
        std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

        // retrieves the collision flags
        collisionFilterGroup = (short) (long long) argumentsMap["collision_filter_group"];
        collisionFilterMask = (short) (long long) argumentsMap["collision_filter_mask"];
    }

    // the physical node rigid body reference
    btRigidBody *physicalNodeRigidBody;

    if(physicalNodeRigidBody = this->physicalNodeRigidBodyMap[physicalNode]) {
        return physicalNodeRigidBody;
    }

    // the collision shape reference
    btCollisionShape *collisionShape;

    // in case a collision node is defined
    if(collisionNode) {
        // retrieves the bullet physics collision solid
        BulletPhysicsEngineCollisionSolid *bulletPhysicsEngineCollisionSolid = (BulletPhysicsEngineCollisionSolid *) collisionNode->getCollisionSolid();

        // retrieves the bullet collision shape
        collisionShape = (btCollisionShape *) bulletPhysicsEngineCollisionSolid->getCollisionShape();
    } else {
        // sets the collision shape as null (invalid)
        collisionShape = NULL;
    }

    // retrieves the physical node position
    Coordinate3d_t &physicalNodePosition = physicalNode->getPosition();

    // converts the physical node position
    btVector3 physicalNodePositionVector(physicalNodePosition.x, physicalNodePosition.y, physicalNodePosition.z);

    // initializes the physical node transform
    btTransform physicalNodeTransform;
    physicalNodeTransform.setIdentity();
    physicalNodeTransform.setOrigin(physicalNodePositionVector);

    // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes "active" objects
    PhysicalNodeMotionState *physicalNodeMotionState = new PhysicalNodeMotionState(physicalNodeTransform, physicalNode);

    // retrieves the physical node mass
    float physicalNodeMass = physicalNode->getMass();

    // retrieves the physical node inertia
    Coordinate3d_t &physicalNodeInertia = physicalNode->getInertia();

    // converts the physical node inertia
    btVector3 physicalNodeInertiaVector(physicalNodeInertia.x, physicalNodeInertia.y, physicalNodeInertia.z);

    // creates the rigid body construction info
    btRigidBody::btRigidBodyConstructionInfo physicalNodeRigidBodyInfo(physicalNodeMass, physicalNodeMotionState, collisionShape, physicalNodeInertiaVector);

    // creates the physical node rigid body
    physicalNodeRigidBody = new btRigidBody(physicalNodeRigidBodyInfo);

    // sets the collision node's collision flags in the corresponding rigid body
    this->setRigidBodyCollisionFlags(physicalNodeRigidBody, collisionNode);

    // adds the rigid body to the dynamics world
    this->dynamicsWorld->addRigidBody(physicalNodeRigidBody, collisionFilterMask, collisionFilterGroup);

    // sets the physical node rigid body in the physical node rigid body map
    this->physicalNodeRigidBodyMap[physicalNode] = physicalNodeRigidBody;

    // sets the physical node rigid body in the rigid body physical node map
    this->rigidBodyPhysicalNodeMap[physicalNodeRigidBody] = physicalNode;

    // returns the physical node rigid body
    return physicalNodeRigidBody;
}

void BulletPhysicsEngine::updatePhysicalNodePosition(PhysicalNode *physicalNode, const Coordinate3d_t &position) {
    // retrieves the rigid body for the current physical node
    btRigidBody *physicalNodeRigidBody = this->physicalNodeRigidBodyMap[physicalNode];

    // retrieves the rigid body's motion state
    PhysicalNodeMotionState *physicalNodeMotionState = (PhysicalNodeMotionState *) physicalNodeRigidBody->getMotionState();

    // updates the motion state's position
    physicalNodeMotionState->setPosition(position);
}

void BulletPhysicsEngine::setRigidBodyCollisionFlags(btRigidBody *rigidBody, CollisionNode *collisionNode) {
    // retrieves the collision flags
    int collisionFlags = rigidBody->getCollisionFlags();

    // determines if the collision node has the contact response options enabled
    if(!collisionNode->getContactResponseEnabled()) {
        // sets the no contact response collision flag
        collisionFlags |= btRigidBody::CF_NO_CONTACT_RESPONSE;
    }

    // sets the determined collision flags
    rigidBody->setCollisionFlags(collisionFlags);
}

void BulletPhysicsEngine::addPhysicalNodeImpulse(PhysicalNode *physicalNode, const Coordinate3d_t &impulse, const Coordinate3d_t &relativePosition) {
    // retrieves the rigid body for the current physical node
    btRigidBody *physicalNodeRigidBody = this->physicalNodeRigidBodyMap[physicalNode];

    // creates the impulse vector
    btVector3 impulseVector = btVector3(impulse.x, impulse.y, impulse.z);

    // creates the relative position vector
    btVector3 relativePositionVector = btVector3(relativePosition.x, relativePosition.y, relativePosition.z);

    // adds an impulse to the physical node
    physicalNodeRigidBody->applyImpulse(impulseVector, relativePositionVector);
}

void BulletPhysicsEngine::setPhysicalNodeVelocity(PhysicalNode *physicalNode, const Coordinate3d_t &velocity) {
    // retrieves the rigid body for the current physical node
    btRigidBody *physicalNodeRigidBody = this->physicalNodeRigidBodyMap[physicalNode];

    // creates the velocity vector
    btVector3 velocityVector = btVector3(velocity.x, velocity.y, velocity.z);

    // adds an impulse to the physical node
    physicalNodeRigidBody->setLinearVelocity(velocityVector);

    // in case the rigid body has been deemed inactive by the constraint solver
    if(physicalNodeRigidBody->getActivationState() == ISLAND_SLEEPING) {
        // activates the rigid body
        physicalNodeRigidBody->setActivationState(ACTIVE_TAG);
    }
}

void BulletPhysicsEngine::setGravity(const Coordinate3d_t &gravity) {
    // sets the gravity
    this->gravity = gravity;

    // creates the gravity vector
    btVector3 gravityVector = btVector3(gravity.x, gravity.y, gravity.z);

    // sets the gravity in the dynamic world
    this->dynamicsWorld->setGravity(gravityVector);
}
