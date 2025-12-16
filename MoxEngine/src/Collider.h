#pragma once
#include "SFML/System/Vector2.hpp"
#include "json.hpp"
#include <iostream>
#include "Event.h"

class GameObject;
class Transform;
enum ColliderType {
	None,
	Box,
	Circle,
	Tilemap
};
struct Manifold;
struct CircleCollider;
struct BoxCollider;




struct Collider {

private:

	Event<Manifold, const Collider*> _onCollisionEnter{};
	Event<Manifold, const Collider*> _onCollisionStay{};
	Event<Manifold, const Collider*> _onCollisionExit{};


public:

	std::vector<Collider*> _prevCollisions;
	std::vector<Collider*> _curCollisions;

	sf::Vector2f _colliderOrigin = { 0,0 };
	sf::Vector2f _halfSize = { 0,0 };
	GameObject* _parent = nullptr;
	Transform* _transform = nullptr;


	sf::Vector2f GetWorldPosition() const;
	




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


	


	static bool BoxVsBox(const BoxCollider& a, const BoxCollider& b);

	static bool BoxVsCircle(const BoxCollider& box, const CircleCollider& circle);

	static bool CircleVsCircle(const CircleCollider& a, const CircleCollider& b);

	static bool PointVsBox(const sf::Vector2f& p, const BoxCollider& box);

	static bool PointVsCircle(const sf::Vector2f& p, const CircleCollider& c);
	/*bool PointVsTilemap(const sf::Vector2f& p, const TilemapCollider& map)
	{
		int tileX = static_cast<int>(p.x) / map.tileSize.x;
		int tileY = static_cast<int>(p.y) / map.tileSize.y;

		return map.isSolid(tileX, tileY);
	}
	*/

	static Manifold BoxVsBoxManifold(const BoxCollider& a, const BoxCollider& b);

	static Manifold CircleVsCircleManifold(const CircleCollider& a, const CircleCollider& b);

	static Manifold BoxVsCircleManifold(const BoxCollider& box, const CircleCollider& circle);

	static Manifold GetManifold(const Collider* a, const Collider* b);


	static bool CheckPoint(const sf::Vector2f& p, const Collider* col);

	static bool CheckCollision(const Collider* a, const Collider* b);

	Event<Manifold, const Collider*>& GetOnCollisionEnter() { return _onCollisionEnter; }
	virtual void OnCollisionEnter(const Collider* other);

	Event<Manifold, const Collider*>& GetOnCollisionExit() { return _onCollisionExit; }
	virtual void OnCollisionExit(const Collider* other);
	
	Event<Manifold, const Collider*>& GetOnCollisionStay() { return _onCollisionStay; }
	virtual void OnCollisionStay(const Collider* other);

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;

	virtual void SaveToJSON(nlohmann::json& data) {

		data["colliderType"] = type;

	}
#endif
};