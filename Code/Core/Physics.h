#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <btBulletDynamicsCommon.h>

class Physics
{
	public:
		Physics();
		~Physics();

	private:
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_broadphase;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_dynamicsWorld;
};

#endif // PHYSICS_H_