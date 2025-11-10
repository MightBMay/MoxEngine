#pragma once
#include "pch.h"


class Component;
class Renderer;
//class Transform;

class GameObject : public sf::Drawable{
public:
	std::unique_ptr<Transform> _transform;
	
private:
	std::vector<std::unique_ptr<Component>> _components;
	std::unique_ptr<Renderer> _renderer;



public:

	template<typename T, typename... Args>
	T& setRenderer(Args&&... args) {
		static_assert(std::is_base_of_v<Renderer, T>, "Type must inherit from renderer");
		_renderer = std::make_unique<T>(std::forward<Args>(args)...);

		return *static_cast<T*>(_renderer.get());
	}

	template<typename T, typename... Args>
	T& addComponent(Args&&... args) {

		static_assert(std::is_base_of_v<Component, T>, "Type must inherit from component");
		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->SetParent(this);
		component->Start();

		T& ref = *component;
		_components.emplace_back(std::move(component));
		
		return ref;


	}

	template<typename T>
	T* GetComponent() const {

		for (auto& c : _components) {
			if (auto casted = dynamic_cast<T*>(c.get()))
				return casted;
		}
		return nullptr;
	}

	Renderer* getRenderer() {
		return _renderer.get();
	}

	GameObject(const sf::Vector2f& position);



	virtual void Update(float deltaTime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};




	