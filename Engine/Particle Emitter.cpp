#include "Particle Emitter.h"

#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Camera.h"
#include "ModuleInput.h"

#include "Firework.h"

#include "imGUI\imgui.h"
#include "OpenGL.h"

// Guide: http://r3dux.org/2010/10/how-to-create-a-simple-fireworks-effect-in-opengl-and-sdl/

Particle_Emitter::Particle_Emitter(GameObject* linkedTo) : Component(linkedTo, C_Particle_Emitter)
{
	char tmp[NAME_MAX_LEN];
	sprintf(tmp, "Particle Emitter##%i", uid);
	name = tmp;
}

void Particle_Emitter::UpdateNow()
{
	DrawTexture();

	if (timer < 1)
	{
		particles.push_back(new Particle(this));
		timer = 2;
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
		ImGui::DragFloat("Speed: ", &counter, 0.001f, 0, 5);
		ImGui::DragFloat("Alpha: ", &alpha, 0.01f, 0, 1);
		ImGui::DragFloat("Blending: ", &blending_over_time, 0.01f, 0, 1);
}

void Particle_Emitter::SaveSpecifics(pugi::xml_node& myNode)
{
}

void Particle_Emitter::LoadSpecifics(pugi::xml_node & myNode)
{
}

void Particle_Emitter::DrawTexture()
{
	for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		(*it)->Update();
}

Particle::Particle(Particle_Emitter* _emitter)
{
	emitter = _emitter;
	timer = 0;
	speed = 0.001f;
	pos = 0;
	max_life = 2.5f;
	size = 1;
}

void Particle::Update()
{
	if (to_destroy)
		//delete[] this;

	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendColor(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, 0);

	//Alpha
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, emitter->alpha);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glMultMatrixf(emitter->object->GetTransform()->GetGlobalTransform().ptr());
	glBegin(GL_QUADS);

	// Bottom right
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(0.2f, pos - 0.2f);

	// Top right
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.2f, pos + 0.2f);

	// Top left
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.2f, pos + 0.2f);

	// Bottom left
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-0.2f, pos - 0.2f);

	glEnd();

	if (emitter->object->HasComponent(Component::C_material))
		glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	//glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	timer += Time.dt/10;

	//pos += speed;

	if (timer > max_life)
	{
		to_destroy = true;
	}
}