#pragma once
#include "pch.h"
#include "Scene.h"
#include "json.hpp"
#include "Component.h"
#include "ComponentFactory.h"
#include "Renderer.h"
#include "RendererFactory.h"

std::unique_ptr<GameObject> LoadGameObjectFromJSON(const nlohmann::json& data) {

	auto obj = std::make_unique<GameObject>();// create object

	// read and assign general object data
	std::string name = data.value("name", "unnamed");
	obj->SetName(name);
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

	if (!data.contains("components")) return obj;
	for (auto& componentData : data["components"]) {
		std::string type = componentData["type"];
		auto component = ComponentFactory::instance().Create(type, componentData);
		component->SetParent(obj.get());
		obj->addComponent(std::move(component));
	}

	return obj;

}

std::unique_ptr<Scene> LoadScene(const nlohmann::json& data) {
	auto scene = std::make_unique<Scene>();

	for (auto& objectData : data["objects"]) {
		auto obj = LoadGameObjectFromJSON(objectData);
		scene->addObject(std::move(obj));
	}
	return scene;
}

