#pragma once
#include "SFML/System/Vector2.hpp"
#include "json.hpp"
enum ColliderType {
	None,
	Box,
	Circle,
	Tilemap
};

struct CircleCollider;
struct BoxCollider;

struct Collider {


	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;

	virtual nlohmann::json SaveToJSON() const = 0;


	static constexpr const char* ToString(const ColliderType type) {
		switch (type) {
			case ColliderType::Box: return "box";
			case ColliderType::Circle: return "circle";
			case ColliderType::Tilemap: return "tilemap";
		}
		return "unknown";
	}

	static ColliderType FromString(const std::string& str) {
	
		if(str == "box") return ColliderType::Box;
		else if (str == "circle") return ColliderType::Circle;
		else if (str == "tilemap") return ColliderType::Tilemap;
		return ColliderType::None;
	}

	ColliderType type;
	virtual ~Collider() = default;

	bool BoxVsBox(const BoxCollider& a, const BoxCollider& b);


	bool BoxVsCircle(const BoxCollider& box, const CircleCollider& circle);

	bool CircleVsCircle(const CircleCollider& a, const CircleCollider& b);

	bool PointVsBox(const sf::Vector2f& p, const BoxCollider& box);

	bool PointVsCircle(const sf::Vector2f& p, const CircleCollider& c);
	/*bool PointVsTilemap(const sf::Vector2f& p, const TilemapCollider& map)
	{
		int tileX = static_cast<int>(p.x) / map.tileSize.x;
		int tileY = static_cast<int>(p.y) / map.tileSize.y;

		return map.isSolid(tileX, tileY);
	}
	*/


	bool CheckPoint(const sf::Vector2f& p, const Collider* col);

	bool CheckCollision(const Collider* a, const Collider* b);
};