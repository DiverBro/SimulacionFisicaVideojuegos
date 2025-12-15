#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "Vector3D.h"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Proyectil.h"
#include "ParticleSystem.h"
#include "Mapa.h"
#include "WindGenerator.h"
#include "ExplosionGenerator.h"

#include <iostream>

std::string display_text = "This is a test";


using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

Particle* part;
vector<Proyectil*> pr;
ParticleSystem* pS;
PxRigidDynamic* proyectil;
Mapa* map;
int fuente = 0;
float angleDeg = 0.0f;
const float angleStep = 2.0f;
 float speed = 15.0f;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	Vector3D x(1, 0, 0);
	Vector3D y(0, 1, 0);
	Vector3D z(0, 0, 1);
	Vector3D centre(0, 0, 0);

	//EJES
	/*RenderItem* render_Item = new RenderItem(CreateShape(PxSphereGeometry(1.0f)),
		new PxTransform(centre.getX(), centre.getY(), centre.getZ()), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	RenderItem* eje1 = new RenderItem(CreateShape(PxSphereGeometry(1.0f)),
		new PxTransform(-x.getX() * 10, -y.getY() * 10, centre.getZ()), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RenderItem* eje2 = new RenderItem(CreateShape(PxSphereGeometry(1.0f)),
		new PxTransform(x.getX() * 10, centre.getY() * 10, centre.getZ()), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RenderItem* eje3 = new RenderItem(CreateShape(PxSphereGeometry(1.0f)),
		new PxTransform(centre.getX(), y.getY() * 10, centre.getZ()), Vector4(0.0f, 1.0f, 0.0f, 1.0f));*/

		//part = new Particle(Vector3D(0, 0, 0), Vector3D(-10, 0, -10), Vector3D(-1, 0, 0), 0.999);

	/*proyectil = new Proyectil(Vector3D(0, 0, 0), Vector3D(0, 0, 0), Vector3D(0, 0, 0),
		10.0f, 10.0f, 10.0f, { 1.0f, 1.0f, 1.0f, 1.0f });*/
	proyectil = gPhysics->createRigidDynamic(PxTransform(0, 0, 0));
	PxShape* shape = CreateShape(PxSphereGeometry(1));
	proyectil->attachShape(*shape);
	//PxRigidBodyExt::updateMassAndInertia(*cube, 0);
	proyectil->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, true);
	gScene->addActor(*proyectil);
	RenderItem* rnd = new RenderItem(shape, proyectil, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	map = new Mapa(0, Vector3D(0, 0, 0), Vector3D(5, 5, 5), proyectil, gPhysics, gScene);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);

	for (Proyectil* p : pr) {
		p->integ(t);
	}

	if (pS)
		pS->update(t, fuente, Vector3D(0, 0, 0));

	if (map)
		map->update(t, angleDeg);

	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
	delete part;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'R':
	{
		angleDeg += angleStep;
		break;
	}
	case 'F':
	{
		angleDeg -= angleStep;
		break;
	}
	case 'T': {
		if (speed < 80)
		speed++;
		break;
	}
	case 'G': {
		if(speed > 15)
		speed--;
		break;
	}
	case ' ':
	{
		if (proyectil)  // Asegurándonos de que el rigidDynamic no sea nulo
		{
			// Verificamos si la velocidad en X e Y es 0, como en tu código
			if (proyectil->getLinearVelocity().x == 0 && proyectil->getLinearVelocity().y == 0)
			{
				// Calculamos el ángulo en radianes, como en tu código original
				float angleRad = angleDeg * 3.14159265f / 180.0f;

				// Calculamos la velocidad en base a la dirección (velocidad * cos/ sin)
				PxVec3 vel(speed * cos(angleRad), speed * sin(angleRad), 0.0f);  // Usamos PxVec3 para la velocidad

				proyectil->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, false);
				proyectil->wakeUp();
				// Asignamos la nueva velocidad al rigidDynamic
				proyectil->setLinearVelocity(vel);

				// Reiniciamos el ángulo después de disparar (igual que en tu código)
				angleDeg = 0;
			}
		}
		break;
	}
	case '0': {
		map->resetLevel(Vector3D(0, 0, 0), Vector3D(5, 5, 5));
		break;
	}
			//PARTICULAS DEFINIDAS CON DISTINTA MASA + TIPOS DE BALAS
	case 'J': {//BALA DE TANQUE
		pr.push_back(new Proyectil(GetCamera()->getTransform().p, GetCamera()->getDir(), Vector3D(0, 0, 0), 1800.0f, 25.0f, 10.0f, { 1.0f, 0.0f, 0.0f, 1.0f }));
		break;
	}
	case'K': {//BALA DE PISTOLA
		pr.push_back(new Proyectil(GetCamera()->getTransform().p, GetCamera()->getDir(), Vector3D(0, 0, 0), 330.0f, 5.0f, 10.0f, { 0.0f, 1.0f, 0.0f, 1.0f }));
		break;
	}
	case'L': {//BALA DE CAÑON
		pr.push_back(new Proyectil(GetCamera()->getTransform().p, GetCamera()->getDir(), Vector3D(0, 0, 0), 250.0f, 15.0f, 10.0f, { 0.0f, 0.0f, 1.0f, 1.0f }));
		break;
	}
	case 'B': {
		delete pS;
		pS = new ParticleSystem();
		fuente = 0;
		break;
	}
	case 'N': {
		delete pS;
		pS = new ParticleSystem();
		fuente = 1;
		break;
	}
	case 'M': {
		delete pS;
		pS = new ParticleSystem();
		fuente = 2;
		break;
	}
	case 'U': {
		if (pS)
			pS->forceVertex("wind", Vector3D(0, 0, 0), Vector3D(20, 0, -20));
		break;
	}
	case 'I': {
		if (pS)
			pS->forceVertex("wind", Vector3D(0, 0, 0), Vector3D(-20, 0, 20));
		break;
	}
	case 'Z': {
		if (pS)
			pS->forceVertex("explosion", Vector3D(0, 10, 0), Vector3D(0, 0, 0));
		break;
	}
	case 'X': {
		if (pS)
			pS->forceVertex("spring", Vector3D(0, 0, 0), Vector3D(0, 1, 0), 1, 10);
		break;
	}
	case 'P': {
		if (pS)
			pS->clearForceVertex();
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}