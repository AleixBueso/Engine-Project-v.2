#ifndef __FIREWORK__
#define __FIREWORK__

#include "Globals.h"

// Guide: http://r3dux.org/2010/10/how-to-create-a-simple-fireworks-effect-in-opengl-and-sdl/

class Firework
{
	uint particles;
	float speed_x, speed_y, speed_z;
	//GLfloat color;
	bool exploded = false;
	float life_time = 0;
	float max_life_time = 10;


public:
	Firework();

	void Start();

	void Update();
};

#endif // !__FIREWORK__
