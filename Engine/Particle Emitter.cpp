#include "Particle Emitter.h"

#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleImporter.h"

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

	object->obb.pos.x += 0.1f;
	object->obb.pos.y += 0.1f;

	DrawTexture();

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

void Particle_Emitter::DrawTexture()
{

	glPushMatrix();
	glMultMatrixf(object->GetTransform()->GetGlobalTransform().ptr());
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	// Bottom right
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(1.0f, -1.0f);

	// Top right
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(1.0f, 1.0f);

	// Top left
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-1.0f, 1.0f);

	// Bottom left
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-1.0f, -1.0f);

	glEnd();

	if (object->HasComponent(Component::C_material))
		glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glDisable(GL_BLEND);

}