#pragma once
#include "Collider.h"
#include "BoxCollider.h"
#include "SFML/System/Vector2.hpp"
#include <algorithm>

struct CircleCollider : Collider {
	sf::Vector2f _center;
	float _radius;

	CircleCollider(const sf::Vector2f& center, float radius) {
		type = ColliderType::Circle;
		_center = center;
		_radius = radius;
	}




};