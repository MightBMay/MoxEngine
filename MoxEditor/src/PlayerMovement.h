#pragma once
#include "Component.h"
#include "Global.h"
#include "RigidBody.h"


class PlayerMovement : public Component {
	
	


protected:
	
	float _acceleration = 100;
	float _deceleration = 250;
	float _gravity = 100;
	Transform* _transform;
	sf::Vector2f direction = sf::Vector2f(0, 0);
	Collider* _collider;
	GameObject* test = nullptr;


	RigidBody* _rb = nullptr;


	void GetMoveDirection() {
		direction.x = Input::GetAction("right") - Input::GetAction("left");
		direction.y = Input::GetAction("down") - Input::GetAction("up");

		if (direction.x != 0.f || direction.y != 0.f)
			direction = direction.normalized();

	}


public:


	static std::unique_ptr<Component> Create(const nlohmann::json& data) {
		bool enabled = data.value("enabled", true);
		uint64_t guid = data.value("guid", GenerateGUID());
		return std::make_unique<PlayerMovement>(
			guid,
			enabled,
			data.value("acceleration", 100),
			data.value("deceleration", 250),
			data.value("gravity", 100)
		);
	}
	virtual void SetParent(GameObject* parent) { 
		_parent = parent; 
		_transform = parent->_transform.get(); 
	}

	virtual void Update(float deltaTime) {

		constexpr float maxHorizontalSpeed = 250.f;

		if (!_rb) return;
		GetMoveDirection();

		auto curVel = _rb->getVelocity();
		
		// Apply deceleration if no horizontal input
		float decelX = 0.f;
		if (direction.x == 0.f) {
			// deceleration opposes current velocity, clamp to zero
			if (curVel.x > 0.f)
				decelX = -std::min(_deceleration * deltaTime, curVel.x);
			else if (curVel.x < 0.f)
				decelX = std::min(_deceleration * deltaTime, -curVel.x);
		}

		// extra deceleration if going too fast.
		if (std::abs(curVel.x) >= maxHorizontalSpeed) {
			decelX += std::copysign(_deceleration * deltaTime, -curVel.x);
		}

		// Horizontal acceleration from input
		float accelX = direction.x * _acceleration * deltaTime;

		// Vertical movement: gravity
		float accelY = _gravity * deltaTime;

		// Update velocity
		sf::Vector2f change = sf::Vector2f(accelX + decelX, accelY);
		_rb->AddVelocity(change);
	
	}




	virtual void Start() override {
		Collider* collider = _parent->getCollider();
		if (!collider) return;

		collider->GetOnCollisionEnter() += [](Manifold m, const Collider* other ) {std::cout << "On Collision Enter: " << other->_parent->GetName()<<"\n"; };
		collider->GetOnCollisionStay()  += [](Manifold m, const Collider* other) {std::cout << "On Collision Stay: " << other->_parent->GetName() << "\n"; };
		collider->GetOnCollisionExit()  += [](Manifold m, const Collider* other) {std::cout << "On Collision Exit: " << other->_parent->GetName() << "\n"; };
	}

	PlayerMovement(uint64_t guid, bool enabled, float accel, float decel, float grav):Component(guid),_acceleration(accel), _deceleration(decel), _gravity(grav){
		_enabled = enabled;
	}
	virtual std::string GetName() const override { static std::string name = "PlayerMovement"; return  name; };

	virtual void ComponentAdded(Component* newComponent) override {
		if (auto* rb = dynamic_cast<RigidBody*>(newComponent)) {
			_rb = rb;
		}
	}

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) override{
		if (!data.contains("speed")) data["speed"] = _acceleration;
		float speed = data.value("speed", _acceleration);
		if (ImGui::InputFloat("speed", &speed)) data["speed"] = speed;

	}

	virtual void getInspectorParams() override {


		std::string label = "Acceleration##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if(ImGui::InputFloat(label.c_str(), &_acceleration));
		label = "Deceleration##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::InputFloat(label.c_str(), &_deceleration));
		label = "Gravity##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::InputFloat(label.c_str(), &_gravity));


	}

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = GetName();
		data["guid"] = GetGUID();
		data["enabled"] = _enabled;
		data["acceleration"] = _acceleration;
		data["deceleration"] = _deceleration;
		data["gravity"] = _gravity;
		return data;
	}
#endif

private:
	inline static bool registered = [] {// static bool with a lambda assignment. ensures this is calculated exactly once, at startup, to register for the SceneLoader's use later.
		ComponentFactory::instance().Register("PlayerMovement", &PlayerMovement::Create);
		return true;
		}();


};