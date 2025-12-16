#pragma once
#include "pch.h"
#include "Scene.h"
#include "json.hpp"
#include "Component.h"
#include "ComponentFactory.h"
#include "Renderer.h"
#include "RendererFactory.h"
#include "SceneHeirarchy.h"

class SceneLoader {
private:
	static std::unique_ptr<GameObject> LoadGameObjectFromJSON(const nlohmann::json& data) {


		uint64_t GUID = data.value("guid", GenerateGUID());

		auto obj = std::make_unique<GameObject>(GUID);// create object

		// read and assign general object data
		std::string name = data.value("name", "unnamed");
		obj->SetName(name);
		obj->SetLayer(data.value("renderLayer", 0));
		sf::Vector2f position(data["position"][0], data["position"][1]);
		sf::Vector2f scale(data["scale"][0], data["scale"][1]);
		float rotation = data["rotation"];
		obj->_transform->SetTransform(position, scale, rotation);


		if (data.contains("renderer") && !data["renderer"].is_null()) {
		// read, create and assign renderer type.
			auto& rendererData = data["renderer"];
			std::string rendererType = rendererData["type"];
			if (rendererType != "None") {

				auto renderer = RendererFactory::instance().Create(rendererType, rendererData);
				renderer->SetTransform(obj->_transform.get()); // manaully set transform.
				obj->setRenderer(std::move(renderer));
			}
		}

		if (data.contains("colliderData")) {
			auto colliderData = data["colliderData"];
			std::string colType = colliderData["type"];
			std::unique_ptr<Collider> collider = ColliderFactory::instance().Create(colType, colliderData);
			obj->setCollider(std::move(collider));
		}


		if (!data.contains("components")) return obj;
		for (auto& componentData : data["components"]) {
			std::string type = componentData["type"];
			auto component = ComponentFactory::instance().Create(type, componentData);
			component->SetParent(obj.get());
			obj->addComponent(std::move(component));
		}


		return obj;

	}

	static std::unique_ptr<Scene> LoadScene(const nlohmann::json& data) {
		auto scene = std::make_unique<Scene>();

		scene->SetName(data.value("sceneName", "unnamed scene"));

		for (auto& objectData : data["objects"]) {
			auto obj = LoadGameObjectFromJSON(objectData);
			scene->addObject(std::move(obj));
		}
		return scene;
	}
public:

	static std::unique_ptr<Scene> LoadSceneFromPath(std::string path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			std::cerr << "failed to open scene \n";
			return nullptr;
		}

		nlohmann::json data;
		try {
			file >> data;
		}
		catch (const nlohmann::json::parse_error& e) {
			std::cerr << "JSON parse error: " << e.what() << "\n";
			return nullptr;
		}


		GUI_SceneHierarchy::instance().ClearSelectedObject();

		std::filesystem::path fsPath(path);
		std::string windowName = fsPath.filename().string();
		window.setTitle("MoxEngine | " + windowName);
		return LoadScene(data);

	}
};
