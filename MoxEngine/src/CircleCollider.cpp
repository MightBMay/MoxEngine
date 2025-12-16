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
	if(ImGui::InputFloat2("Center", raw) ){
		_colliderOrigin.x = raw[0];
		_colliderOrigin.y = raw[1];
	}


}



void CircleCollider::SaveToJSON(nlohmann::json& data)  {
	nlohmann::json colliderData;
	colliderData["type"] = "circle";
	colliderData["radius"] = _radius;
	colliderData["origin"] = { _colliderOrigin.x, _colliderOrigin.y };
	data["colliderData"] = colliderData;
}


#endif
