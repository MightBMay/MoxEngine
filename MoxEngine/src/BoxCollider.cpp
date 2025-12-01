#include "BoxCollider.h"
#include "Transform.h"
//#include "pch.h"


const sf::Vector2f BoxCollider::GetSize() const {
	sf::Vector2f scale = _transform->GetScale();
	return { _size.x * scale.x, _size.y * scale.y };
}


#if IN_EDITOR
#include "ImGuiFileDialog.h"



void BoxCollider::getImGuiParams(nlohmann::json& data) {


}
void BoxCollider::getInspectorParams() {

	float rawSize[2] = { _size.x, _size.y };
	ImGui::InputFloat2("Size##ColliderRadius", rawSize);


	float raw[2] = { _colliderOrigin.x,_colliderOrigin.y }; // DEBUG / TODO : rn this doesnt actually modify the origin.
	ImGui::InputFloat2("Center", raw);
	if (ImGui::IsItemDeactivatedAfterEdit()) {

	}

}

nlohmann::json BoxCollider::SaveToJSON() const {
	nlohmann::json data;
	data["type"] = "circle";
	data["size"] = { _size.x, _size.y };
	data["centerPos"] = { _colliderOrigin.x, _colliderOrigin.y };
	return data;

}


#endif


