#pragma once
#include "GameObject.h"
#include "json.hpp"



class Component {

protected:
	GameObject* _parent = nullptr;
	bool _enabled = true;

public: 
	void SetEnabled(bool value) { _enabled = value; }
	bool GetEnabled() { return _enabled; }

	virtual void SetParent(GameObject* parent) { _parent = parent; }
	virtual ~Component() = default;
	virtual void Start(){}
	virtual void Update(float deltaTime){}
	virtual std::string GetName() const = 0;
	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;

	virtual nlohmann::json SaveToJSON() const = 0;

};