#pragma once
#include "Component.h"
#include "ComponentFactory.h"
#include "SFML/System/Vector2.hpp"

class RigidBody : public Component {

private:

	sf::Vector2f _velocity = { 0,0 };



public:

	bool IsKinematic = true;
	void SetVelocity(sf::Vector2f& newV) { _velocity = newV; }
	void SetVelocity(float x, float y) { _velocity = { x,y }; }

	void AddVelocity(sf::Vector2f& add) { _velocity += add; }
	void AddVelocity(float x, float y) { _velocity += {x, y}; }

	static std::unique_ptr<Component> Create(const nlohmann::json& data) {

		return std::make_unique<RigidBody>();

	}

	virtual void Update(float deltaTime) override {

		//if (_velocity.lengthSquared() <= 0) return;

		_parent->MoveAndCollide(_velocity*deltaTime);
	}

	void CollisionCancelVelocity(const Manifold& m, const Collider* other) {
		const sf::Vector2f& n = m.normal;

		if (std::abs(n.x) > std::abs(n.y)) { // find dominant axis.

			SetVelocity(0, _velocity.y);
		}
		else {
			SetVelocity(_velocity.x, 0);
		}


	}

	const sf::Vector2f& getVelocity() const { return _velocity; }

	virtual std::string GetName() const { return "RigidBody"; }

#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data) override{}
	virtual void getInspectorParams() override;

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = "RigidBody";
		return data;
	}
#endif

private:

	inline static bool registered = []{
		ComponentFactory::instance().Register("RigidBody", &RigidBody::Create);
		return true;
		}();

};