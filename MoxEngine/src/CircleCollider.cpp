#include "CircleCollider.h"

#if IN_EDITOR
#include "ImGuiFileDialog.h"



void CircleCollider::getImGuiParams(nlohmann::json& data) {


}
void CircleCollider::getInspectorParams() {

	ImGui::InputFloat("Radius", &_radius);
	float raw[2] = { _center.x,_center.y };
	ImGui::InputFloat2("Center", raw);

}

nlohmann::json CircleCollider::SaveToJSON() const {
	nlohmann::json data;
	data["type"] = "circle";
	data["radius"] = _radius;
	data["centerPos"] = { _center.x, _center.y };
	return data;

}


#endif
