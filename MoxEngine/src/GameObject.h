#pragma once
#include "pch.h"
#include "json.hpp"
#include "Renderer.h"

class Component;
class Renderer;
class Collider;
//class Transform;

class GameObject : public sf::Drawable{
public:
	std::unique_ptr<Transform> _transform;

	
private:
	std::vector<std::unique_ptr<Component>> _components;
	std::unique_ptr<Renderer> _renderer = nullptr;
	std::unique_ptr<Collider> _collider = nullptr;
	int _renderLayer = 0;
	std::string _name = "unnamed";
	uint64_t _guid;



public:

	template<typename T, typename... Args>
	T& setRenderer(Args&&... args) {
		static_assert(std::is_base_of_v<Renderer, T>, "Type must inherit from renderer");
		_renderer = std::make_unique<T>(std::forward<Args>(args)...);
		_renderer->AfterAddedToGameObject();
		return *static_cast<T*>(_renderer.get());
	}

	void setRenderer(std::unique_ptr<Renderer> renderer) {
		_renderer = std::move(renderer);
		_renderer->AfterAddedToGameObject();
	}

	std::unique_ptr<Renderer> RemoveRenderer() {
		return std::move(_renderer);
	}




	template<typename T, typename... Args>
	T& setCollider(Args&&... args) {
		static_assert(std::is_base_of_v<Collider, T>, "Type must inherit from collider");
		_collider = std::make_unique<T>(std::forward<Args>(args)...);
		return *static_cast<T*>(_collider.get());
	}

	void setCollider(std::unique_ptr<Collider> collider) {
		_collider = std::move(collider);
	}

	std::unique_ptr<Collider> removeCollider() {
		return std::move(_collider);
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

	void addComponent(std::unique_ptr<Component> component);


	std::unique_ptr<Component> removeComponent(Component* toRemove) {
		auto it = std::find_if(_components.begin(), _components.end(),
			[toRemove](const std::unique_ptr<Component>& ptr) { return ptr.get() == toRemove; });

		if (it != _components.end()) {
			// Move the unique_ptr out
			std::unique_ptr<Component> removed = std::move(*it);
			// Erase the element from the vector
			_components.erase(it);
			return removed;
		}

		return nullptr; // not found
	}


	template<typename T>
	T* GetComponent() const {

		for (auto& c : _components) {
			if (auto casted = dynamic_cast<T*>(c.get()))
				return casted;
		}
		return nullptr;

	}


	std::vector<Component*> getComponents(){
		std::vector<Component*> result;
		result.reserve(_components.size());
		for (auto& c : _components)
			result.push_back(c.get());
		return result;
	}

	Renderer* getRenderer() {
		return _renderer.get();
	}

	

	GameObject();
	GameObject(uint64_t GUID);
	GameObject(int renderLayer, const sf::Vector2f& position);



	void SetName(std::string name) { _name = name; }
	std::string GetName() const { return _name; }


	int GetLayer() { return _renderLayer; }
	void SetLayer(int value) { _renderLayer = value; }

	const uint64_t GetGUID() const { return _guid; }


	virtual void Update(float deltaTime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

#if IN_EDITOR
	nlohmann::json SaveToJSON() const;
#endif

};




	