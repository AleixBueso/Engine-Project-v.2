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
}

void Particle_Emitter::UpdateNow(const float3& point, const float3& _up)
{
	for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//(*it)->transform->Update();
		(*it)->Update(point, _up);
	}

	if (timer > 3)
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

}

Particle::Particle(Particle_Emitter* _emitter)
{
	transform = new Transform(nullptr);
	emitter = _emitter;
	timer = 0;
	speed = 0.001f;
	pos = 0;
	max_life = 2.5f;
	size = 1;
}

void Particle::Update(const float3& point, const float3& _up)
{
	if (to_destroy)
		delete[] this;

	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendColor(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, 0);

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
	


	/*float3 front = App->camera->GetDefaultCam()->object->GetTransform()->GetGlobalPos() - emitter->object->GetTransform()->GetGlobalPos();
	 float4x4 tmp = transformation.LookAt(float3(1,0,0), front, float3(0, 1, 0), float3(0, 1, 0));
	 float3 rotation = tmp.ToEulerXYX() * RADTODEG;

	 while (rotation.x < 0) { rotation.x += 360; }
	 while (rotation.y < 0) { rotation.y += 360; }
	 while (rotation.z < 0) { rotation.z += 360; }
	 transformation = math::Quat::FromEulerXYX(rotation.x, rotation.y, rotation.z);
	 //float4x4 tmp = transformation.Translate(float3(2, 0, 3));
	 //transformation = tmp;
	//}
	*/
	//Alpha
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, emitter->alpha);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glMultMatrixf(transform->GetGlobalTransform().ptr());

	glBegin(GL_TRIANGLES);

	// Top right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, 0, +0.2f);

	// Bottom right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 0, - 0.2f);

	// Top left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0, +0.2f);

	// Top left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0, + 0.2f);

	// Bottom right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 0, -0.2f);

	// Bottom left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 0, - 0.2f);

	glEnd();

	if (emitter->object->HasComponent(Component::C_material))
	{
		emitter->object->GetComponent<Material>().front()->GetTexture(particle_tex);
		glBindTexture(GL_TRIANGLES, particle_tex);
		glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}

	//glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	timer += Time.dt;

	pos += speed;

	if (timer > max_life)
	{
		to_destroy = true;
	}
}