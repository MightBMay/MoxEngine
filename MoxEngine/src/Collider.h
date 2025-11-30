#pragma once

enum ColliderType {
	Box,
	Circle,
	Tilemap
};


struct Collider {

	static constexpr const char* ToString(const ColliderType type) {
		switch (type) {
			case ColliderType::Box: return "box";
			case ColliderType::Circle: return "circle";
			case ColliderType::Tilemap: return "tilemap";
		}
		return "unknown";
	}

	ColliderType type;
	virtual ~Collider() = default;

	bool BoxVsBox(const BoxCollider& a, const BoxCollider& b) {
		return !(a._position.x + a._size.x < b._position.x ||
			b._position.x + b._size.x < a._position.x ||
			a._position.y + a._size.y < b._position.y ||
			b._position.y + b._size.y < a._position.y);
	}


	bool BoxVsCircle(const BoxCollider& box, const CircleCollider& circle)
	{
		float cx = std::clamp(circle._center.x, box._position.x, box._position.x + box._size.x);
		float cy = std::clamp(circle._center.y, box._position.y, box._position.y + box._size.y);

		float dx = circle._center.x - cx;
		float dy = circle._center.y - cy;

		return (dx * dx + dy * dy) <= circle._radius * circle._radius;
	}

	bool CircleVsCircle(const CircleCollider& a, const CircleCollider& b)
	{
		float dx = a._center.x - b._center.x;
		float dy = a._center.y - b._center.y;
		float rad = a._radius + b._radius;
		return (dx * dx + dy * dy) <= rad * rad;
	}

	bool PointVsBox(const sf::Vector2f& p, const BoxCollider& box)
	{
		return (p.x >= box._position.x &&
			p.x <= box._position.x + box._size.x &&
			p.y >= box._position.y &&
			p.y <= box._position.y + box._size.y);
	}

	bool PointVsCircle(const sf::Vector2f& p, const CircleCollider& c)
	{
		float dx = p.x - c._center.x;
		float dy = p.y - c._center.y;
		return (dx * dx + dy * dy) <= (c._radius * c._radius);
	}

	/*bool PointVsTilemap(const sf::Vector2f& p, const TilemapCollider& map)
	{
		int tileX = static_cast<int>(p.x) / map.tileSize.x;
		int tileY = static_cast<int>(p.y) / map.tileSize.y;

		return map.isSolid(tileX, tileY);
	}
	*/


	bool CheckPoint(const sf::Vector2f& p, const Collider* col)
	{
		switch (col->type)
		{
			case ColliderType::Box:
				return PointVsBox(p, *(BoxCollider*)col);

			case ColliderType::Circle:
				return PointVsCircle(p, *(CircleCollider*)col);

			/*case ColliderType::Tilemap:
				return PointVsTilemap(p, *(TilemapCollider*)col);*/
		}
		return false;
	}

	bool CheckCollision(const Collider * a, const Collider * b) {
	if (a->type == ColliderType::Box && b->type == ColliderType::Box)
		return BoxVsBox(*(BoxCollider*)a, *(BoxCollider*)b);

	else if (a->type == ColliderType::Circle && b->type == ColliderType::Circle)
		return CircleVsCircle(*(CircleCollider*)a, *(CircleCollider*)b);

	else if (a->type == ColliderType::Box && b->type == ColliderType::Circle)
		return BoxVsCircle(*(BoxCollider*)a, *(CircleCollider*)b);

	else if (a->type == ColliderType::Circle && b->type == ColliderType::Box)
		return BoxVsCircle(*(BoxCollider*)b, *(CircleCollider*)a);

	/*else if (a->type == ColliderType::Box && b->type == ColliderType::Tilemap)
		return BoxVsTilemap(*(BoxCollider*)a, *(TilemapCollider*)b);

	else if (a->type == ColliderType::Tilemap && b->type == ColliderType::Box)
		return BoxVsTilemap(*(BoxCollider*)b, *(TilemapCollider*)a);*/


	return false;
	}
};