#include "BoxCollider.h"
#include "Transform.h"



const sf::Vector2f BoxCollider::GetSize() const {
	if (!_transform) return { _size.x, _size.y };
	sf::Vector2f scale = _transform->GetScale();
	return { _size.x * (scale.x), _size.y * (scale.y) };
}


#if IN_EDITOR
#include "ImGuiFileDialog.h"



void BoxCollider::getImGuiParams(nlohmann::json& data) {


}


void BoxCollider::getInspectorParams() {

	float rawSize[2] = { _size.x, _size.y };
	if(ImGui::InputFloat2("Size##ColliderRadius", rawSize) ){
		_size.x = rawSize[0];
		_size.y = rawSize[1];
	}


	float raw[2] = { _colliderOrigin.x,_colliderOrigin.y }; // DEBUG / TODO : rn this doesnt actually modify the origin.
	if(ImGui::InputFloat2("Center", raw)) {
		_colliderOrigin.x = raw[0];
		_colliderOrigin.y = raw[1];
	}

}

void BoxCollider::SaveToJSON(nlohmann::json& data) {
	nlohmann::json colliderData;
	colliderData["type"] = "box";
	colliderData["size"] = { _size.x, _size.y };
	colliderData["origin"] = { _colliderOrigin.x, _colliderOrigin.y };
	data["colliderData"] = colliderData;
}


#endif


