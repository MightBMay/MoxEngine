#include "ComponentFactory.h"
#include "Component.h"
#include "pch.h"

void ComponentFactory::Register(const std::string& type, std::unique_ptr<Component>(*createFunction)(const nlohmann::json&)) {
	registry[type] = createFunction;
	registeredTypes.emplace_back(type);
}

std::unique_ptr<Component> ComponentFactory::Create(const std::string& type, const nlohmann::json& data) {
	auto it = registry.find(type);
	if (it != registry.end())
		return it->second(data);
	std::cout << "huh\n";
	throw std::runtime_error("Component type not found in registry:" + type);
}
