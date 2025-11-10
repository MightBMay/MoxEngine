#pragma once
#include "Component.h"
#include "Global.h"

class PlayerMovement : public Component {

public:
	virtual void Start() { }
	virtual void Update(float deltaTime) {
		static Transform* transform = _parent->_transform.get();


		
		auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		int sign = _parent->getRenderer()->IsPointInside(mousePos) ? 1 : -1;
		transform->Move({sign*50*deltaTime, 0});
		
	
	
	
	}

};