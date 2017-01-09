#ifndef __PARTICLE_EMITTER__
#define __PARTICLE_EMITTER__

#include "ResourcedComponent.h"
#include "Firework.h"
#include <list>
class Particle_Emitter;

class Particle
{
public:

	float timer;
	float speed;
	float pos;
	float max_life;
	float size;

	bool to_destroy = false;

	Particle_Emitter* emitter;
	
	Particle(Particle_Emitter* _emitter);

	void Update();

private:

};

class Particle_Emitter :public Component
{
public:
	Particle_Emitter(GameObject* linkedTo);

	void UpdateNow();

	void EditorContent();

	void SaveSpecifics(pugi::xml_node& myNode);
	void LoadSpecifics(pugi::xml_node& myNode);

	void DrawTexture();

	static Type GetType() { return Type::C_Particle_Emitter; }

	float counter = 0;
	float speed = 0.001f;
	float alpha = 0;
	float blending_over_time = 1;
	float timer = 0;

	std::list<Particle*> particles;
};

#endif
