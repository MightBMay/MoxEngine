#include "pch.h"
#include "GameObject.h"
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
	_components.emplace_back(std::move(component));

}
void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (_renderer == nullptr) return;
	states.transform *= _transform->GetSFTransform();
	_renderer->draw(target, states);

}




void GameObject::MoveAndCollide(sf::Vector2f delta) {

	   // X axis
	float moveX = delta.x;
	if (moveX != 0.f)
		MoveAndCollideAxis(moveX, true);

	// Y axis
	float moveY = delta.y;
	if (moveY != 0.f)
		MoveAndCollideAxis(moveY, false);
}

void GameObject::MoveAndCollideAxis(float amount, bool isXAxis)
{
	// Try full move
	_transform->Move(isXAxis ? sf::Vector2f{ amount, 0.f }
	: sf::Vector2f{ 0.f, amount });

	Collider* self = getCollider();

	for (Collider* other : CollisionSystem::GetColliders())
	{
		if (other == self )//|| other->isTrigger)
			continue;

		Manifold m = Collider::GetManifold(self, other);
		if (!m.hit)
			continue;

		// Determine push direction
		float sign = (amount > 0.f) ? -1.f : 1.f;

		if (isXAxis)
		{
			_transform->Move({ sign * m.penetration, 0.f });
		}
		else
		{
			_transform->Move({ 0.f, sign * m.penetration });
		}

		// Stop movement on this axis
		break;
	}
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