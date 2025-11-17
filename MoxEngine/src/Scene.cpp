#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

#if IN_EDITOR
void Scene::Save() {

	nlohmann::json sceneJson = {
		{"sceneName", _sceneName},
		{"objects", nlohmann::json::array()}
	};

	for (auto& obj : _objects) {
		sceneJson["objects"].push_back(obj->SaveToJSON());
	}

	// Write to file
	
	std::ofstream file("../assets/scenes/" + _sceneName);
	if (file.is_open()) {
		file << sceneJson.dump(4); // pretty print with 4-space indentation
	}
}
#endif