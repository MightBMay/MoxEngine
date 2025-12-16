#pragma once
#include "Global.h"

class GameObject;

class Renderer {

protected:
	Transform* _transform = nullptr;
	sf::Drawable* _drawable = nullptr;

public:
	virtual ~Renderer() = default;
	Renderer(){}
	Renderer(GameObject* obj);
	Renderer(Transform* transform);


	virtual void AfterAddedToGameObject() {

	}


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
#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;

	virtual nlohmann::json SaveToJSON() const = 0;
#endif


};