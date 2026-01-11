#pragma once
#include "Component.h"
#include "Global.h"
#include "RigidBody.h"


class PlayerMovement : public Component {
	
	


protected:
	
	float _acceleration = 100;
	float _deceleration = 250;
	float _gravity = 100;
	float _jumpHeight = 100;
	float _maxHSpeed = 250.f;
	float _maxVSpeed = 100;
	bool _isGrounded = true;
	Transform* _transform;
	sf::Vector2f direction = sf::Vector2f(0, 0);
	Collider* _collider;
	GameObject* test = nullptr;


	RigidBody* _rb = nullptr;


	void GetMoveDirection() {
		direction.x = Input::GetAction("right") - Input::GetAction("left");
		direction.y = Input::GetAction("down") - Input::GetAction("up");

		if (direction.x != 0.f && direction.y != 0.f)
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
			data.value("gravity", 100),
			data.value("jumpHeight", 100),
			data.value("maxHSpeed", 100),
			data.value("maxVSpeed", 100)
		);
	}
	virtual void SetParent(GameObject* parent) { 
		_parent = parent; 
		_transform = parent->_transform.get(); 
	}

	virtual void Update(float deltaTime) {

		if (!_rb) return;
		GetMoveDirection();

		auto& curVel = _rb->getVelocity();
		
		// Apply deceleration if no horizontal input
		float decelX = 0.f;
		if (direction.x == 0.f) {
			// deceleration opposes current velocity, clamp to zero
			if (curVel.x > 0.f)
				decelX = -std::min(_deceleration*1.8f * deltaTime, curVel.x);
			else if (curVel.x < 0.f)
				decelX = std::min(_deceleration *1.8f* deltaTime, -curVel.x);
		}


		// extra deceleration if going too fast. lets you go faster if u have a faster acceleration source.
		if (std::abs(curVel.x) >= _maxHSpeed) {
			decelX += std::copysign(_deceleration * deltaTime, -curVel.x);
		} 
		if (curVel.y > _maxVSpeed) { // clamp fall speed to maxVSpeed.
			_rb->SetVelocity(curVel.x, _maxVSpeed);
		}

		// Horizontal acceleration from input
		float accelX = direction.x * _acceleration * deltaTime;

		// Vertical movement: gravity
		float accelY = 0;
		if(!_isGrounded) accelY += _gravity * deltaTime;


		if (direction.x != 0.f && std::signbit(curVel.x) != std::signbit(direction.x)) {
			
			_rb->SetVelocity(-curVel.x, curVel.y);
		}

		// Update velocity
		sf::Vector2f change = sf::Vector2f(accelX + decelX, accelY);
		_rb->AddVelocity(change);

		if (Input::GetKeyDown(sf::Keyboard::Scan::Space) && _isGrounded) {
			_isGrounded = false;
			float jumpVelocity = std::sqrt(2 * _gravity * _jumpHeight);
			_rb->SetVelocity(curVel.x, -jumpVelocity );
		}
	
	}

	virtual void Start() override {
		_collider = _parent->getCollider();

	}

	PlayerMovement(uint64_t guid, bool enabled, float accel, float decel, float grav, float jumpheight, float maxH, float maxV) :
		Component(guid), _acceleration(accel), _deceleration(decel), _gravity(grav),_jumpHeight(jumpheight), _maxHSpeed(maxH), _maxVSpeed(maxV) {
		_enabled = enabled;
	}
	virtual std::string GetName() const override { static std::string name = "PlayerMovement"; return  name; };

	virtual void OnComponentAdded(Component* newComponent) override {
		if (auto* rb = dynamic_cast<RigidBody*>(newComponent)) {
			_rb = rb;
		}
	}

	virtual void OnColliderAdded(Collider* newCollider) override {
		_collider = newCollider;
		_collider->GetOnCollisionEnter() += [this](const Manifold& m, const Collider* other) {
			std::cout << "onc Enter\n";
			_rb->CollisionCancelVelocity(m, other);
			_isGrounded = GroundCheck(m.normal.y);// set grounded if collision entered is beneath.
			};
		_collider->GetOnCollisionStay() += [this](const Manifold& m, const Collider* other) { 
			std::cout << "onc STAY\n";
			};

		_collider->GetOnCollisionExit() += [this](const Collider* other) {
			//_isGrounded = false;// CheckStillGrounded();
			std::cout << "onc Exit\n";
			};


	}

	virtual bool GroundCheck(const float& collisionNormalY) const {
		return collisionNormalY <= -0.5;

	}

	bool CheckStillGrounded() const {
		/*const auto& Contacts = CollisionSystem::GetContacts();

		for (const Contact& contact : Contacts) {
			Collider* other = nullptr;

			if (contact.a == _collider) other = contact.b;
			else if (contact.b == _collider) other = contact.a;
			else continue;

			if (contact.m.hit && GroundCheck(contact.m.normal.y)) {
				return true;
			}
		}

		return false;*/
		return false;
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

		label = "Jump Height##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::InputFloat(label.c_str(), &_jumpHeight));

		label = "Max Horizontal Speed##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::InputFloat(label.c_str(), &_maxHSpeed));

		label = "Max Vertical Speed##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::InputFloat(label.c_str(), &_maxVSpeed));

		label = "Is Grounded##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::Checkbox(label.c_str(), &_isGrounded));



	}

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = GetName();
		data["guid"] = GetGUID();
		data["enabled"] = _enabled;
		data["acceleration"] = _acceleration;
		data["deceleration"] = _deceleration;
		data["gravity"] = _gravity;
		data["jumpHeight"] = _jumpHeight;
		data["maxHSpeed"] = _maxHSpeed;
		data["maxVSpeed"] = _maxVSpeed;
		return data;
	}
#endif

private:
	inline static bool registered = [] {// static bool with a lambda assignment. ensures this is calculated exactly once, at startup, to register for the SceneLoader's use later.
		ComponentFactory::instance().Register("PlayerMovement", &PlayerMovement::Create);
		return true;
		}();


};