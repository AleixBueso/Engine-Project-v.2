#ifndef __FIREWORK__
#define __FIREWORK__

#include "Globals.h"
#include "Particle Emitter.h"
#include <list>

// Guide: http://r3dux.org/2010/10/how-to-create-a-simple-fireworks-effect-in-opengl-and-sdl/

class Firework
{
	std::list<Particle_Emitter*> firework_emitters;



public:
	Firework();
	~Firework();

	void CreateFirework();

	void Update(const float3& point, const float3& _up);
};

#endif //__FIREWORK__

