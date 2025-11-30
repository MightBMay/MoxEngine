#pragma once
#include "Component.h"
#include "Global.h"



class PlayerMovement : public Component {
	
	


protected:
	float _moveSpeed = 50;
	Transform* _transform;
	sf::Vector2f direction = sf::Vector2f(0, 0);

	GameObject* test = nullptr;

public:

	static std::unique_ptr<Component> Create(const nlohmann::json& data) {
		bool enabled = data.value("enabled", true);
		uint64_t guid = data.value("guid", GenerateGUID());

		return std::make_unique<PlayerMovement>(guid, enabled, data.value("speed", 50));
	}
	virtual void SetParent(GameObject* parent) { 
		_parent = parent; 
		_transform = parent->_transform.get(); 
	}
	virtual void Update(float deltaTime) {
		direction.x = 0;
		direction.y = 0;
		if (Input::GetAction("up") && !Input::GetAction("down"))
			direction.y = -1;
		else if (Input::GetAction("down") && !Input::GetAction("up"))
			direction.y = 1;

		if (Input::GetAction("right") && !Input::GetAction("left"))
			direction.x = 1;
		else if (Input::GetAction("left") && !Input::GetAction("right"))
			direction.x = -1;
	
		if (direction.lengthSquared() <= 0) return; // if invalid direction vector, do not normalize and move, just exit.

		direction = direction.normalized();

		_transform->Move(direction * _moveSpeed * deltaTime);

	
	}

	PlayerMovement(uint64_t guid, bool enabled, float speed = 50):Component(guid),_moveSpeed(speed){
		_enabled = enabled;
	}
	virtual std::string GetName() const override { static std::string name = "PlayerMovement"; return  name; };

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) override{
		if (!data.contains("speed")) data["speed"] = _moveSpeed;
		float speed = data.value("speed", _moveSpeed);
		if (ImGui::InputFloat("speed", &speed)) data["speed"] = speed;
	}

	virtual void getInspectorParams() override {


		std::string label = "PlayerMovement##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		ImGui::InputFloat(label.c_str(), &_moveSpeed);
	}

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = GetName();
		data["guid"] = GetGUID();
		data["enabled"] = _enabled;
		data["speed"] = _moveSpeed;
		return data;
	}
#endif

private:
	inline static bool registered = [] {// static bool with a lambda assignment. ensures this is calculated exactly once, at startup, to register for the SceneLoader's use later.
		ComponentFactory::instance().Register("PlayerMovement", &PlayerMovement::Create);
		return true;
		}();


};