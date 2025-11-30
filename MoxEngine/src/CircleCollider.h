#pragma once
#include "Collider.h"
#include "ColliderFactory.h"
#include "SFML/System/Vector2.hpp"
#include <algorithm>


struct CircleCollider : Collider {
	sf::Vector2f _center = {0,0};
	float _radius = 0;

	static std::unique_ptr<Collider> Create(const nlohmann::json& data) {
		std::unique_ptr<CircleCollider> col = std::make_unique<CircleCollider>();
		col->type = ColliderType::Circle;
		col->_radius = data.value("radius", 0);
		if (data.contains("centerPos")) {
			col->_center.x = data["centerPos"][0];
			col->_center.y = data["centerPos"][1];
		}
		
	}

	CircleCollider() = default;



#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) override;
	virtual void getInspectorParams() override;

	virtual nlohmann::json SaveToJSON() const override;


#endif


private:
	inline static bool registered = [] {
		ColliderFactory::instance().Register("circle", &CircleCollider::Create);
		return true;
		}();


};