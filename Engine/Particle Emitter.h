#ifndef __PARTICLE_EMITTER__
#define __PARTICLE_EMITTER__

#include "ResourcedComponent.h"

class Particle_Emitter :public Component
{
public:
	Particle_Emitter(GameObject* linkedTo);

	void UpdateNow(const float3& point, const float3& _up);

	void EditorContent();

	void SaveSpecifics(pugi::xml_node& myNode);
	void LoadSpecifics(pugi::xml_node& myNode);

	static Type GetType() { return Type::C_Particle_Emitter; }

};

#endif
