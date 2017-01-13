#ifndef __PARTICLE_EMITTER__
#define __PARTICLE_EMITTER__

#include "ResourcedComponent.h"
#include "Transform.h"
//#include "Firework.h"
#include "Particle Emitter.h"
#include <list>

class Particle_Emitter;

class Particle
{
public:

	float timer;
	float speed;
	float max_life;
	float size;
	float3 direction;

	bool is_firework = false;

	uint particle_tex = 0;

	float3 localForward = float3(0, 0, 1);
	float3 localUp = float3(0, 1, 0);

	Transform* transform;

	bool to_destroy = false;

	Particle_Emitter* emitter;
	
	Particle(Particle_Emitter* _emitter);
	~Particle();

	void Update(const float3& point, const float3& _up);

private:

};

class Particle_Emitter :public Component
{
public:
	Particle_Emitter(GameObject* linkedTo, bool IsFirework = false);

	void UpdateNow(const float3& point, const float3& _up);

	void EditorContent();

	void SaveSpecifics(pugi::xml_node& myNode);
	void LoadSpecifics(pugi::xml_node& myNode);

	void DrawTexture();

	static Type GetType() { return Type::C_Particle_Emitter; }

	float alpha = 0;
	float timer = 0;
	float time_to_spawn = 0.5f;
	float gravity[3];

	std::list<Particle*> particles;

	// Firework System
	bool is_firework = false;
	bool first_particle_launch = false;
	bool exploded = false;
	bool explosion_created = false;
	float3 explosion_pos = float3(0, 0, 0);
};

#endif
