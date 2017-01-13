#include "Firework.h"
#include "Globals.h"
#include "OpenGL.h"

#include "Application.h"
#include "ModuleGOmanager.h"
#include "Particle Emitter.h"

Firework::Firework()
{

};

Firework::~Firework()
{

};

void Firework::CreateFirework()
{
	Particle_Emitter* tmp = new Particle_Emitter(nullptr, true);
	firework_emitters.push_back(tmp);
	tmp->gravity[1] = 8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (8 - 15)));
}

void Firework::Update(const float3& point, const float3& _up)
{
	for (std::list<Particle_Emitter*>::iterator it = firework_emitters.begin(); it != firework_emitters.end(); it++)
		(*it)->UpdateNow(point, _up);
}