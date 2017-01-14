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

#include <ctime>

// Guide: http://r3dux.org/2010/10/how-to-create-a-simple-fireworks-effect-in-opengl-and-sdl/

Particle_Emitter::Particle_Emitter(GameObject* linkedTo, bool IsFirework) : Component(linkedTo, C_Particle_Emitter)
{
	char tmp[NAME_MAX_LEN];
	sprintf(tmp, "Particle Emitter##%i", uid);
	name = tmp;
	is_firework = IsFirework;
	gravity[0] = 0;
	gravity[1] = 0.5f;
	gravity[2] = 0;
	exploded = false;
	//object->GetTransform()->allowRotation = false;
}

void Particle_Emitter::UpdateNow(const float3& point, const float3& _up)
{
	if (is_firework)
	{
		if (first_particle_launch == false)
		{
			particles.push_back(new Particle(this));
			first_particle_launch = true;
		}

		if (exploded && explosion_created == false)
		{
			uint max_particles = 20;
			for (uint i = 0; i < max_particles; i++)
				particles.push_back(new Particle(this));
			explosion_created = true;
			gravity[1] = -0.2f;
		}
	}

	else
	{
		if (timer > time_to_spawn)
		{
			particles.push_back(new Particle(this));
			timer = 0;
		}
	}

	for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		(*it)->Update(point, _up);


	std::list<Particle*>::iterator it = particles.begin();
	while (it != particles.end())
	{
		if ((*it)->to_destroy)
		{
			it = particles.erase(it);
		}

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
		if (emitter->exploded)
			speed = 2;
		else
			speed = 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.01f - 0.001f)));
		max_life = 7 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - 7)));
		size = 1;
		direction = float3(1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-1 - 1))), 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-1 - 1))), 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-1 - 1))));
}

Particle::~Particle()
{
	delete[] transform;
}

void Particle::Update(const float3& point, const float3& _up)
{
	// Billboard
	if (_up.IsZero() == false)
	{
		Transform* trans;

		if(emitter->is_firework)
			trans = transform;
		else
			trans = emitter->object->GetTransform();

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

	// Alpha
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
	glAlphaFunc(GL_GREATER, emitter->alpha);

	if(!emitter->is_firework)
		glEnable(GL_TEXTURE_2D);

	// Color
	if(emitter->is_firework)
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glPushMatrix();

	if (!emitter->is_firework)
	{
		math::float4x4 final_transform = transform->GetGlobalTransform() * emitter->object->GetTransform()->GetGlobalTransform();
		glMultMatrixf(*final_transform.v);
	}

	else
	{
		glMultMatrixf(*transform->GetGlobalTransform().v);
	}


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

	if (!emitter->is_firework)
	{
		//Setting texture
		if (emitter->object)
		{
			if (emitter->object->HasComponent(Component::C_material))
			{
				particle_tex = emitter->object->GetComponent<Material>().front()->GetTexture(particle_tex);
				//glBindTexture(GL_TEXTURE_2D, particle_tex);
				glBindTexture(GL_TRIANGLES, particle_tex);
				glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			}
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	timer += Time.dt;



	//Calculating directions
	float3 dir_speed = float3(direction.x * speed + emitter->gravity[0], direction.y * speed + emitter->gravity[1], direction.z * speed + emitter->gravity[2]);

	float3 new_position;

	if (emitter->emitter_pos.y != 0 && !position_changed)
	{
		new_position.Set(emitter->emitter_pos.x + transform->GetGlobalPos().x + dir_speed.x * Time.dt, emitter->emitter_pos.y + transform->GetGlobalPos().y + dir_speed.y * Time.dt, emitter->emitter_pos.z + transform->GetGlobalPos().z + dir_speed.z * Time.dt);
		position_changed = true;
	}

	else
		new_position.Set(transform->GetGlobalPos().x + dir_speed.x * Time.dt, transform->GetGlobalPos().y + dir_speed.y * Time.dt, transform->GetGlobalPos().z + dir_speed.z * Time.dt);

	transform->SetGlobalPos(new_position);
	transform->UpdateGlobalTransform();


	if (timer > max_life && to_destroy == false)
	{
		if (emitter->is_firework && emitter->exploded == false)
		{
			emitter->emitter_pos = this->transform->GetGlobalPos();
			emitter->exploded = true;
		}
			to_destroy = true;
	}
}