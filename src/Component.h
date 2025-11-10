#pragma once
#include "pch.h"



class Component {

protected:
	GameObject* _parent = nullptr;

public: 
	void SetParent(GameObject* parent) { _parent = parent; }
	virtual ~Component() = default;
	virtual void Start(){}
	virtual void Update(float deltaTime){}

};