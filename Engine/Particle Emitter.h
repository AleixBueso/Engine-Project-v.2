#ifndef __PARTICLE_EMITTER__
#define __PARTICLE_EMITTER__

#include "ResourcedComponent.h"
#include "Firework.h"
#include <list>

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
};

#endif
