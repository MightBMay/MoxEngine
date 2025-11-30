#pragma once
#include "Collider.h"
#include "json.hpp"

using ColCreateFunc = std::unique_ptr<Collider>(*)(const nlohmann::json&);
class ColliderFactory {
private:
	std::unordered_map<ColliderType, ColCreateFunc> registry;
	std::vector <ColliderType>registeredTypes;

public:
	static ColliderFactory& instance() {
		static ColliderFactory inst;
		return inst;
	}


	const std::vector<ColliderType> GetTypes() {
		return registeredTypes;
	}

	void Register(const ColliderType& type, ColCreateFunc function) {
		registry[type] = function;
		registeredTypes.emplace_back(type);

	}

	std::unique_ptr<Collider> Create(const ColliderType& type, const nlohmann::json& data) {
		auto it = registry.find(type);
		if (it != registry.end()) 
			return it->second(data);
		throw std::runtime_error("Collider type not found in registry. ");// +Collider::ToString(type) );
	}
};