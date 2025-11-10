#pragma once
#include "pch.h"

class Renderer {

protected:
	Transform* _transform = nullptr;
	sf::Drawable* _drawable = nullptr;


public:
	virtual ~Renderer() = default;
	Renderer(GameObject* obj) { _transform = obj->_transform.get(); }
	Renderer(Transform* transform) { _transform = transform; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual sf::FloatRect GetGlobalBounds() const = 0;

	virtual bool IsPointInside(sf::Vector2f point) {
		return GetGlobalBounds().contains(point);
	}

	virtual bool IsPointInside(sf::Vector2i point) {

		return GetGlobalBounds().contains( sf::Vector2f(point) );
	}


};