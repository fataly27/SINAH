#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <btBulletDynamicsCommon.h>

#include <memory>

class Physics
{
	public:
		Physics();
		~Physics();

	private:
		unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		unique_ptr<btCollisionDispatcher> m_dispatcher;
		unique_ptr<btBroadphaseInterface> m_broadphase;
		unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
};

#endif // PHYSICS_H_
