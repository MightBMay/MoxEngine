#include "pch.h"
#include "Scene.h"
#include "GameObject.h"


void Scene::SaveToFile(const std::string& filename) {
	nlohmann::json sceneJson = {
		{"scene name", filename},
		{"objects", nlohmann::json::array()}
	};

	for (auto& obj : _objects) {
		sceneJson["objects"].push_back(obj->SaveToJSON());
	}

	// Write to file
	std::ofstream file("../assets/scenes/" + filename);
	if (file.is_open()) {
		file << sceneJson.dump(4); // pretty print with 4-space indentation
	}
}