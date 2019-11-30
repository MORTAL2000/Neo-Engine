#pragma once

#include "Systems/System.hpp"
#include "Engine.hpp"

#include "BulletCubeRigidBodyComponent.hpp"
#include "RegisterBulletComponent.hpp"

#include "ext/bullet3/btBulletDynamicsCommon.h"

using namespace neo;

class BulletSystem : public System {

public:

    btBroadphaseInterface* mBroadphase;
    btCollisionDispatcher* mDispatcher;
    btConstraintSolver* mSolver;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btDynamicsWorld* mDynamicsWorld;

    BulletSystem() :
        System("Bullet System")
    {}

    virtual void init() override {
        mCollisionConfiguration = new btDefaultCollisionConfiguration();
        mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
        mBroadphase = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
        mSolver = sol;
        mDynamicsWorld = new
            btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver,
                mCollisionConfiguration);
        mDynamicsWorld->setGravity(btVector3(0, -4, 0));


        btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -50, 0));
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(0), myMotionState, groundShape, btVector3(0, 0, 0));
        btRigidBody* body = new btRigidBody(rbInfo);
        mDynamicsWorld->addRigidBody(body);
    }

    virtual void update(const float dt) override {
        /* Init new components */
        for (auto& comp : Engine::getComponents<RegisterBulletComponent>()) {
            auto spatial = comp->getGameObject().getComponentByType<SpatialComponent>();
            NEO_ASSERT(spatial, "Attempting to register a bullet body without a SpatialComponent");

            btTransform objTransform;
            objTransform.setFromOpenGLMatrix(glm::value_ptr(spatial->getModelMatrix()));

            if (auto cube = comp->getGameObject().getComponentByType<BulletCubeRigidBodyComponent>()) {
                cube->startTransform.setOrigin(btVector3(
                    spatial->getPosition().x,
                    spatial->getPosition().y,
                    spatial->getPosition().z));
                cube->myMotionState->setWorldTransform(objTransform);
                cube->body->setMotionState(cube->myMotionState);

                mDynamicsWorld->addRigidBody(cube->body);
            }

            Engine::removeComponent<RegisterBulletComponent>(*comp);
        }

        /* Run simulation */
        mDynamicsWorld->stepSimulation(dt);

        /* Update spatials */
        for (auto& cube : Engine::getComponentTuples<BulletCubeRigidBodyComponent, SpatialComponent>()) {
            btTransform worldTransform;
            cube->get<BulletCubeRigidBodyComponent>()->body->getMotionState()->getWorldTransform(worldTransform);


            glm::mat4 M;
            worldTransform.getOpenGLMatrix(glm::value_ptr(M));
            cube->get<SpatialComponent>()->setPosition(glm::vec3(M[3][0], M[3][1], M[3][2]));
            cube->get<SpatialComponent>()->setOrientation(glm::mat3(M));
        }
    }

    virtual void shutdown() override {
        for (int i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0
            ; i--)
        {
            btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()
                [i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) {
                delete body->getMotionState();
            }
            mDynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }
        delete mDynamicsWorld;
        delete mSolver;
        delete mBroadphase;
        delete mDispatcher;
        delete mCollisionConfiguration;
    }

};
