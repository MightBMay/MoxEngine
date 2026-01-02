#include "RigidBody.h"


#if IN_EDITOR
#include "imgui.h"
#endif

void RigidBody::getInspectorParams(){


	if(ImGui::Checkbox("Is Kinematic? ", &isKinematic));

	float velocityRaw[2] = { _velocity.x, _velocity.y };
	if (ImGui::InputFloat2("velocity", velocityRaw)) SetVelocity(velocityRaw[0], velocityRaw[1]);

}