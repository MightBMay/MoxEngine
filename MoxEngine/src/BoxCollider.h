#pragma once
#include "Collider.h"
#include "ColliderFactory.h"
#include "SFML/System/Vector2.hpp"

struct BoxCollider : Collider {
	sf::Vector2f _size = {0,0};


	static std::unique_ptr<Collider> Create(const nlohmann::json& data) {
		std::unique_ptr<BoxCollider> col = std::make_unique<BoxCollider>();
		col->type = ColliderType::Box;
		if (data.contains("size")) {
			col->_size.x = data["size"][0];
			col->_size.y = data["size"][1];
		}
		
		return col;
	}


	const sf::Vector2f GetSize() const;

	BoxCollider() = default;

#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data) override;
	virtual void getInspectorParams() override;

	virtual nlohmann::json SaveToJSON() const override;
#endif



private:
	static inline bool registered = [] {
		ColliderFactory::instance().Register("box", &BoxCollider::Create);
		return true;
		}();

};

