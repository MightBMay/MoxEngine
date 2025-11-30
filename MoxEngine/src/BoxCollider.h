#pragma once
#include "Collider.h"
#include "SFML/System/Vector2.hpp"

struct BoxCollider : Collider {
	sf::Vector2f _position;
	sf::Vector2f _size;


	BoxCollider(const sf::Vector2f& pos, const sf::Vector2f& size){
		type = ColliderType::Box;
		_position = pos;
		_size = size;
	}




	

};

