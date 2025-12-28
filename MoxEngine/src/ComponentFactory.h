#pragma once
#include "json.hpp"
class Component;
class ComponentFactory {

private:
	std::unordered_map<std::string, std::unique_ptr<Component>(*)(const nlohmann::json&)> registry;
	std::vector<std::string> registeredTypes;

public:
	static ComponentFactory& instance() {
		static ComponentFactory inst;
		return inst;
	}
	const std::vector<std::string>& GetTypes() {
		return registeredTypes;
	}

	void Register(const std::string& type, std::unique_ptr<Component>(*createFunction)(const nlohmann::json&));

	std::unique_ptr<Component> Create(const std::string& type, const nlohmann::json& data);


};
