#pragma once
#include "SFML/System/Vector2.hpp"
#include "json.hpp"
#include <iostream>
#include "Event.h"

class GameObject;
class Transform;
enum ColliderType {
	Box,
	Circle,
	Tilemap,
	None,
};
struct Manifold;
struct CircleCollider;
struct BoxCollider;
struct TileMapCollider;
class TileMap;


struct Manifold;
struct Collider;
using ManifoldFn = Manifold(*)(const Collider&, const Collider&);


struct Collider {
protected:
	static const inline  float EPSILON = 1000.f;
private:

	Event<Manifold, const Collider*> _onCollisionEnter{};
	Event<Manifold, const Collider*> _onCollisionStay{};
	Event<const Collider*> _onCollisionExit{};

	bool _isTrigger = false;



private:
	static Manifold Dispatch(const Collider& a, const Collider& b);

	static Manifold BoxBox(const Collider& a, const Collider& b);
	static Manifold BoxCircle(const Collider& a, const Collider& b);
	static Manifold CircleBox(const Collider& a, const Collider& b);
	static Manifold CircleCircle(const Collider& a, const Collider& b);
	static Manifold BoxTilemap(const Collider& a, const Collider& b);
	static Manifold CircleTilemap(const Collider& a, const Collider& b);

	static ManifoldFn collisionDispatch[3][3];



public:


	static Manifold Collide(const Collider& a, const Collider& b);

	std::vector<Collider*> _prevCollisions;
	std::vector<Collider*> _curCollisions;

	sf::Vector2f _colliderOrigin = { 0,0 };
	sf::Vector2f _halfSize = { 0,0 };
	sf::Vector2f _backupPosition = { 0,0 };
	GameObject* _parent = nullptr;
	Transform* _transform = nullptr;


	sf::Vector2f GetWorldPosition() const;
	
	virtual void OnAddedToGameObject(){}

	bool IsTrigger() { return _isTrigger; }

	void SetIsTrigger(bool value) { _isTrigger = value; }


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

	//static sf::IntRect GetTileRange( const sf::FloatRect& bounds,int cellSize) {
	//	return {
	//		int(bounds.left / cellSize),
	//		int(bounds.top / cellSize),
	//		int(std::ceil(bounds.width / cellSize)) + 1,
	//		int(std::ceil(bounds.height / cellSize)) + 1
	//	};
	//}


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

	static Manifold BoxVsBoxManifold(const BoxCollider& a, Transform* aTrans, const BoxCollider& b);

	static Manifold CircleVsCircleManifold(const CircleCollider& a, const CircleCollider& b);

	static Manifold CircleVsTilemapManifold(const CircleCollider& box, const TileMapCollider& tilemap);

	static Manifold BoxVsCircleManifold(const BoxCollider& box, const CircleCollider& circle);

	static Manifold BoxVsTilemapManifold(const BoxCollider& box, const TileMapCollider& tilemap);

	static Manifold GetManifold(const Collider* a, const Collider* b);


	static bool CheckPoint(const sf::Vector2f& p, const Collider* col);

	static bool CheckCollision(const Collider* a, const Collider* b);

	Event<Manifold, const Collider*>& GetOnCollisionEnter() { return _onCollisionEnter; }
	virtual void OnCollisionEnter(const Collider* other, const Manifold& m);

	Event<const Collider*>& GetOnCollisionExit() { return _onCollisionExit; }
	virtual void OnCollisionExit(const Collider* other);
	
	Event<Manifold, const Collider*>& GetOnCollisionStay() { return _onCollisionStay; }
	virtual void OnCollisionStay(const Collider* other, const Manifold& m);

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) = 0;
	virtual void getInspectorParams() = 0;

	virtual void SaveToJSON(nlohmann::json& data) {

		data["colliderType"] = type;

	}
#endif
};