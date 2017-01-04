#include "Particle Emitter.h"

#include "GameObject.h"
#include "Transform.h"
#include "Application.h"

#include "imGUI\imgui.h"
#include "OpenGL.h"

Particle_Emitter::Particle_Emitter(GameObject* linkedTo) : Component(linkedTo, C_Particle_Emitter)
{
	char tmp[NAME_MAX_LEN];
	sprintf(tmp, "Particle Emitter##%i", uid);
	name = tmp;
}

void Particle_Emitter::UpdateNow()
{
	/*if (_up.IsZero() == false)
	{
		Transform* trans = object->GetTransform();
		float3 front = point - trans->GetGlobalPos();

		float4x4 tmp = float4x4::LookAt(object localForward.Normalized(), front, localUp.Normalized(), _up);
		trans->SetGlobalRot(tmp.ToEulerXYZ() * RADTODEG);
	}
	else
	{
		object->GetTransform()->LookAt(point);
	}*/
	
	glPushMatrix();
	glMultMatrixf(object->GetTransform()->GetGlobalTransform().ptr());
	glBegin(GL_QUADS); // 2x2 pixels
	glVertex3f(-1.0f, 1.0f, 0);
	glVertex3f(-1.0f, -1.0f, 0);
	glVertex3f(1.0f, -1.0f, 0);
	glVertex3f(1.0f, 1.0f, 0);
	glEnd();

	if (object->HasComponent(Component::C_material))
		glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glPopMatrix();
}

void Particle_Emitter::EditorContent()
{

}

void Particle_Emitter::SaveSpecifics(pugi::xml_node& myNode)
{
}

void Particle_Emitter::LoadSpecifics(pugi::xml_node & myNode)
{
}