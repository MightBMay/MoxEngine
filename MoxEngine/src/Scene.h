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
		_drawOrder.clear();
		_drawOrder.reserve(_objects.size());
		for (auto& obj : _objects) _drawOrder.push_back(obj.get());
		
		std::sort(_drawOrder.begin(), _drawOrder.end(),
			[](GameObject* a, GameObject* b) { return a->GetLayer() < b->GetLayer(); });


		bool usingUIView = true; // used to determine when to switch sf::views.
		window.setView(defaultView); // start with default view to draw UI in screen space.

		for (auto* obj : _drawOrder) {
			bool isUI = obj->GetLayer() < -127 || obj->GetLayer() > 128; // UI is any object with layer <-127 or >128. 

			// switch view if different from the current one
			if (isUI != usingUIView) { 
				window.setView(isUI ? defaultView : playerView);
				usingUIView = isUI;
			}

			window.draw(*obj);
		}

	}


	void SaveToFile(const std::string& filename);


private:
	std::vector<std::unique_ptr<GameObject>> _objects;
	std::vector<GameObject*> _drawOrder;
};