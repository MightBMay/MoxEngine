#include "GameObject.h"
#include "pch.h"
#include "CollisionSystem.h"


GameObject::GameObject() {
	_transform = std::make_unique<Transform>();
	_guid = GenerateGUID();
}

GameObject::GameObject(uint64_t GUID) :_guid(GUID){
	_transform = std::make_unique<Transform>();
}


GameObject::GameObject(int renderLayer, const sf::Vector2f& position)
{
	_transform = std::make_unique<Transform>();
	_transform->SetPosition(position);
	_guid = GenerateGUID();

}










void GameObject::Update(float deltaTime) {

	for (auto& c : _components) {
		if (c->GetEnabled()) {
			c->Update(deltaTime);
		}
	}
}


void GameObject::addComponent(std::unique_ptr<Component> component) {
	component->SetParent(this);
	component->Start();

	auto componentRaw = component.get();

	for (auto& comp : _components) {
		comp->ComponentAdded(componentRaw);
	}

	_components.emplace_back(std::move(component));

}
void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (_renderer == nullptr) return;
	states.transform *= _transform->GetSFTransform();
	_renderer->draw(target, states);

}




void GameObject::MoveAndCollide(sf::Vector2f delta) {

	if (!_collider) return;
	CollisionSystem::MoveKinematic(*_transform, _collider.get(), delta);
}






#if IN_EDITOR

nlohmann::json GameObject::SaveToJSON() const {
	sf::Vector2f pos = _transform->GetPosition();
	sf::Vector2f scale = _transform->GetScale();
	nlohmann::json data = {
		{ "name", _name },
		{ "guid", _guid},
		{ "renderLayer", _renderLayer},
		{ "position", { pos.x, pos.y } },
		{ "scale", { scale.x, scale.y } },
		{ "rotation", _transform->GetRotationDeg() },
		{ "renderer", _renderer ? _renderer->SaveToJSON() : nlohmann::json{} },
		{ "components", nlohmann::json::array() },
	};

	for (auto& comp : _components) {
		data["components"].push_back(comp->SaveToJSON());
	}
	if (_collider) {
		_collider->SaveToJSON(data);
	}



	return data;
}
#endif