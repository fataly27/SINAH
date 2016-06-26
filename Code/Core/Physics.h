#ifndef PHYSICS_H_
#define PHYSICS_H_

//Includes Bullet 2.83
#include <btBulletDynamicsCommon.h>

#include <memory>

#include "Constants.h"

class Physics
{
	public:
		Physics();
		~Physics();

	private:
		std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> m_dispatcher;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
};

#endif // PHYSICS_H_
