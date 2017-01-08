#include "Firework.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleGOmanager.h"

Firework::Firework()
{

};

void Firework::Start()
{

}

void Firework::Update()
{
	life_time += Time.dt;

	if (life_time >= max_life_time)
	{
		//Erase the list pointer
		std::list<Firework*>::iterator it = App->GO->fireworks.begin();
		while (it != App->GO->fireworks.end())
		{
			if ((*it) == this)
			{
				App->GO->fireworks.erase(it);
				break;
			}
			else
				it++;
		}

		//Delete the firework
		delete(this);
	}
}