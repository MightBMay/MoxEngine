#include "CircleCollider.h"
#include "Transform.h"







float CircleCollider::GetRadius() const {
	auto scale = _transform->GetScale();
	return _radius * std::max(scale.x, scale.y);
}


#if IN_EDITOR
#include "ImGuiFileDialog.h"



void CircleCollider::getImGuiParams(nlohmann::json& data) {


}
void CircleCollider::getInspectorParams() {

	ImGui::InputFloat("Radius##ColliderRadius", &_radius);
	float raw[2] = { _colliderOrigin.x,_colliderOrigin.y }; // DEBUG / TODO : rn this doesnt actually modify the origin.
	ImGui::InputFloat2("Center", raw);
	if (ImGui::IsItemDeactivatedAfterEdit()) {

	}

}



nlohmann::json CircleCollider::SaveToJSON() const {
	nlohmann::json data;
	data["type"] = "circle";
	data["radius"] = _radius;
	data["centerPos"] = { _colliderOrigin.x, _colliderOrigin.y };
	return data;

}


#endif
