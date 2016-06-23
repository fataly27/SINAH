#include "Physics.h"

Physics::Physics() : m_collisionConfiguration(new btDefaultCollisionConfiguration), m_dispatcher(new btCollisionDispatcher(m_collisionConfiguration.get())), m_broadphase(new btDbvtBroadphase),
					 m_solver(new btSequentialImpulseConstraintSolver), m_dynamicsWorld(new btDiscreteDynamicsWorld(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get())) {}
Physics::~Physics() {}