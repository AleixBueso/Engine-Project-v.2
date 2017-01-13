#include "Particle Emitter.h"

#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Camera.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleGOmanager.h"
#include "Material.h"

#include "Firework.h"

#include "imGUI\imgui.h"
#include "OpenGL.h"

// Guide: http://r3dux.org/2010/10/how-to-create-a-simple-fireworks-effect-in-opengl-and-sdl/

Particle_Emitter::Particle_Emitter(GameObject* linkedTo) : Component(linkedTo, C_Particle_Emitter)
{
	char tmp[NAME_MAX_LEN];
	sprintf(tmp, "Particle Emitter##%i", uid);
	name = tmp;
	gravity[0] = 0;
	gravity[1] = 0.5f;
	gravity[2] = 0;
}

void Particle_Emitter::UpdateNow(const float3& point, const float3& _up)
{
	for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//(*it)->transform->Update();
		(*it)->Update(point, _up);
	}

	if (timer > time_to_spawn)
	{
		particles.push_back(new Particle(this));
		timer = 0;
	}

	std::list<Particle*>::iterator it = particles.begin();
	while (it != particles.end())
	{
		if ((*it)->to_destroy)
			it = particles.erase(it);
		else
			it++;
	}

	timer += Time.dt;
}

void Particle_Emitter::EditorContent()
{
		ImGui::DragFloat("Spawn Rate", &time_to_spawn, 0.01f, 0, 1);
		ImGui::DragFloat3("Gravity", gravity, 0.001f);
		ImGui::DragFloat("Alpha", &alpha, 0.01f, 0, 1);
}

void Particle_Emitter::SaveSpecifics(pugi::xml_node& myNode)
{

}

void Particle_Emitter::LoadSpecifics(pugi::xml_node & myNode)
{
}

void Particle_Emitter::DrawTexture()
{

}

Particle::Particle(Particle_Emitter* _emitter)
{
	//Randomizing values;
	transform = new Transform(nullptr);
	emitter = _emitter;
	timer = 0;
	speed = 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.01f - 0.001f)));
	max_life = 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 15)));;
	size = 1;
	direction = float3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

Particle::~Particle()
{
	delete[] transform;
}

void Particle::Update(const float3& point, const float3& _up)
{
	if (to_destroy)
		delete[] this;

	// Billboard
	if (_up.IsZero() == false)
	{
		Transform* trans = transform;
		float3 front = point - trans->GetGlobalPos();

		float4x4 tmp = float4x4::LookAt(localForward.Normalized(), front, localUp.Normalized(), _up);
		trans->SetGlobalRot(tmp.ToEulerXYZ() * RADTODEG);
		transform->UpdateGlobalTransform();
	}
	else
	{
		transform->LookAt(point);
	}
	
	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendColor(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, 0);

	//Alpha
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
	glAlphaFunc(GL_GREATER, emitter->alpha);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glMultMatrixf(*transform->GetGlobalTransform().v);

	//Drawing particle
	glBegin(GL_TRIANGLES);

	//Triangle 1
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.2f, -0.2f, 0.2f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.2f, -0.2, 0.2f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.2f);

	//Triangle 2
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.2f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.2f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.2f, -0.2f, 0.2f);

	glEnd();

	//Setting texture
	if (emitter->object->HasComponent(Component::C_material))
	{
		emitter->object->GetComponent<Material>().front()->GetTexture(particle_tex);
		glBindTexture(GL_TRIANGLES, particle_tex);
		glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	timer += Time.dt;

	//Calculating directions
	float3 dir_speed = float3(direction.x * speed + emitter->gravity[0], direction.y * speed + emitter->gravity[1], direction.z * speed + emitter->gravity[2]);
	float3 new_position(transform->GetGlobalPos().x + dir_speed.x * Time.dt, transform->GetGlobalPos().y + dir_speed.y * Time.dt, transform->GetGlobalPos().z + dir_speed.z * Time.dt);
	transform->SetGlobalPos(new_position);
	transform->UpdateGlobalTransform();

	if (timer > max_life)
	{
		to_destroy = true;
	}
}