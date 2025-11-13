#pragma once
#include "pch.h"
#include "Global.h"
class Renderer {

protected:
	Transform* _transform = nullptr;
	sf::Drawable* _drawable = nullptr;


public:
	virtual ~Renderer() = default;
	Renderer(){}
	Renderer(GameObject* obj) { _transform = obj->_transform.get(); }
	Renderer(Transform* transform) { _transform = transform; }


	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;

	void SetTransform(Transform* transform) {
		_transform = transform;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
		target.draw(*_drawable, states);
	}

	virtual sf::FloatRect GetGlobalBounds() const = 0;

	virtual bool IsPointInside(sf::Vector2f point) {
		return GetGlobalBounds().contains(point);
	}

	virtual bool IsPointInside(sf::Vector2i point) {

		return GetGlobalBounds().contains( sf::Vector2f(point) );
	}


	virtual nlohmann::json SaveToJSON() const = 0;


};