#pragma once
#include "GameObject.h"
#include "json.hpp"
#include "config.h"



struct Collider;
class Renderer;

class Component {
protected:
	GameObject* _parent = nullptr;
	bool _enabled = true;
	uint64_t _guid;

public: 
	void SetEnabled(bool value) { _enabled = value; }
	bool GetEnabled() { return _enabled; }

	virtual void SetParent(GameObject* parent) { _parent = parent; }
	Component() { _guid = GenerateGUID(); }
	Component(uint64_t guid) :_guid(guid) {}
	virtual ~Component() = default;
	virtual void Start(){}
	virtual void Update(float deltaTime){}

	const GameObject* GetParent() const{ return _parent; }

	const uint64_t GetGUID() const { return _guid;  }

	virtual std::string GetName() const = 0;

	virtual void OnComponentAdded(Component* newComponent) {};
	virtual void OnColliderAdded(Collider* newCollider) {};
	virtual void OnRendererAdded(Renderer* newRenderer) {};


#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;
	virtual nlohmann::json SaveToJSON() const = 0;
#endif
};