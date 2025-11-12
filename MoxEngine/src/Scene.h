#pragma once

#include "pch.h"
#include "Global.h"
class Scene {
public:
	Scene() = default;

	void addObject(std::unique_ptr<GameObject> obj) {
		_objects.emplace_back(std::move(obj));

	}

	std::unique_ptr<GameObject> removeObject(GameObject* obj) {
		auto it = std::find_if(_objects.begin(), _objects.end(), // iterate objects list, checking each unique_ptr's underlying
			[obj](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == obj; }); // raw pointer if it is equal to the param.
		if (it != _objects.end()) {
			std::unique_ptr<GameObject> removed = std::move(*it);
			// Erase the element from the vector
			_objects.erase(it);
			return removed;
		}
		return nullptr;
	}

	const std::vector<std::unique_ptr<GameObject>>& getObjects() const {
		return _objects;
	}

	void Update(float deltaTime) {
		for (auto& obj : _objects) {
			obj->Update(deltaTime);
		}
	}

	void Draw() {
		for (auto& obj : _objects) {
			window.draw(*obj, sf::RenderStates::Default);
		}
	}


	void SaveToFile(const std::string& filename);


private:
	std::vector<std::unique_ptr<GameObject>> _objects;
};