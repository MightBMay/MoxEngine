#include "Collider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "TilemapCollider.h"
#include "GameObject.h"
#include "CollisionSystem.h"

sf::Vector2f Collider::GetWorldPosition() const {
	sf::Vector2f scale = _transform->GetScale();
	sf::Vector2f offset{ _colliderOrigin.x * scale.x, _colliderOrigin.y * scale.y };
	return _transform->GetPosition() + offset;
}

bool Collider::BoxVsBox(const BoxCollider& a, const BoxCollider& b) {

	auto centerA = a.GetWorldPosition();
	auto sizeA = a.GetSize();

	auto centerB = b.GetWorldPosition();
	auto sizeB = b.GetSize();

	return !(centerA.x + sizeA.x < centerB.x ||
		centerB.x + sizeB.x < centerA.x ||
		centerA.y + sizeA.y < centerB.y ||
		centerB.y + sizeB.y < centerA.y);
}


bool Collider::BoxVsCircle(const BoxCollider& box, const CircleCollider& circle)
{

	auto circleCenter = circle.GetWorldPosition();
	auto boxCenter = box.GetWorldPosition();
	auto boxSize = box.GetSize();

	float cx = std::clamp(circleCenter.x, boxCenter.x, boxCenter.x + boxSize.x);
	float cy = std::clamp(circleCenter.y, boxCenter.y, boxCenter.y + boxSize.y);

	float dx = circleCenter.x - cx;
	float dy = circleCenter.y - cy;

	float radius = circle.GetRadius();
	return (dx * dx + dy * dy) <= radius * radius;
}

bool Collider::CircleVsCircle(const CircleCollider& a, const CircleCollider& b)
{
	const sf::Vector2f centerA = a.GetWorldPosition();
	const sf::Vector2f centerB = b.GetWorldPosition();

	float dx = centerA.x - centerB.x;
	float dy = centerA.y - centerB.y;
	float rad = a._radius + b._radius;
	return (dx * dx + dy * dy) <= rad * rad;
}

bool Collider::PointVsBox(const sf::Vector2f& p, const BoxCollider& box)
{

	auto boxCenter = box.GetWorldPosition();
	auto boxSize = box.GetSize();

	return (p.x >= boxCenter.x &&
		p.x <= boxCenter.x + boxSize.x &&
		p.y >= boxCenter.y &&
		p.y <= boxCenter.y + boxSize.y);
}

bool Collider::PointVsCircle(const sf::Vector2f& p, const CircleCollider& c)
{
	auto circleCenter = c.GetWorldPosition();
	float radius = c.GetRadius();

	float dx = p.x - circleCenter.x;
	float dy = p.y - circleCenter.y;
	return (dx * dx + dy * dy) <= (radius * radius);
}

/*bool PointVsTilemap(const sf::Vector2f& p, const TilemapCollider& map)
{
	int tileX = static_cast<int>(p.x) / map.tileSize.x;
	int tileY = static_cast<int>(p.y) / map.tileSize.y;

	return map.isSolid(tileX, tileY);
}
*/




Manifold Collider::BoxVsBoxManifold(const BoxCollider& a, const BoxCollider& b)
{
	sf::Vector2f aMin = a.GetWorldPosition();
	sf::Vector2f aMax = aMin + a.GetSize();

	sf::Vector2f bMin = b.GetWorldPosition();
	sf::Vector2f bMax = bMin + b.GetSize();

	float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
	float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

	if (overlapX <= 0.f || overlapY <= 0.f)
		return {};

	Manifold m;
	m.hit = true;

	if (overlapX < overlapY)
	{
		m.penetration = overlapX;
		m.normal = (aMin.x < bMin.x) ? sf::Vector2f{ -1.f, 0.f }
		: sf::Vector2f{ 1.f, 0.f };
	}
	else
	{
		m.penetration = overlapY;
		m.normal = (aMin.y < bMin.y) ? sf::Vector2f{ 0.f, -1.f }
		: sf::Vector2f{ 0.f,  1.f };
	}

	return m;
}


Manifold Collider::CircleVsCircleManifold(const CircleCollider& a, const CircleCollider& b)
{
	sf::Vector2f diff = a.GetWorldPosition() - b.GetWorldPosition();
	float dist2 = diff.x * diff.x + diff.y * diff.y;

	float radius = a.GetRadius() + b.GetRadius();
	float radius2 = radius * radius;

	if (dist2 >= radius2)
		return {};

	float dist = std::sqrt(dist2);

	Manifold m;
	m.hit = true;
	m.penetration = radius - dist;
	m.normal = (dist != 0.f) ? diff / dist : sf::Vector2f{ 1.f, 0.f };

	return m;
}

Manifold Collider::CircleVsTilemapManifold(const CircleCollider& box, const TileMapCollider& tilemap) {



}

Manifold Collider::BoxVsCircleManifold(const BoxCollider& box, const CircleCollider& circle)
{
	sf::Vector2f boxMin = box.GetWorldPosition();
	sf::Vector2f boxMax = boxMin + box.GetSize();
	sf::Vector2f center = circle.GetWorldPosition();

	sf::Vector2f closest{
		std::clamp(center.x, boxMin.x, boxMax.x),
		std::clamp(center.y, boxMin.y, boxMax.y)
	};

	sf::Vector2f diff = center - closest;
	float dist2 = diff.x * diff.x + diff.y * diff.y;
	float radius = circle.GetRadius();

	if (dist2 > radius * radius)
		return {};

	Manifold m;
	m.hit = true;

	float dist = std::sqrt(dist2);
	if (dist != 0.f)
	{
		m.normal = diff / dist;          // normalized direction from box to circle center
		m.penetration = radius - dist;   // how far circle overlaps
	}
	else
	{
		// circle center exactly inside box (rare corner case)
		// pick arbitrary axis aligned normal
		float left = center.x - boxMin.x;
		float right = boxMax.x - center.x;
		float top = center.y - boxMin.y;
		float bottom = boxMax.y - center.y;

		if (std::min(left, right) < std::min(top, bottom))
			m.normal = (left < right) ? sf::Vector2f{ -1.f, 0.f } : sf::Vector2f{ 1.f, 0.f };
		else
			m.normal = (top < bottom) ? sf::Vector2f{ 0.f, -1.f } : sf::Vector2f{ 0.f, 1.f };

		m.penetration = radius;
	}

	// Optional: axis alignment fix for discrete solver
	if (std::abs(m.normal.x) > std::abs(m.normal.y))
		m.normal = sf::Vector2f{ (m.normal.x > 0 ? 1.f : -1.f), 0.f };
	else
		m.normal = sf::Vector2f{ 0.f, (m.normal.y > 0 ? 1.f : -1.f) };

	return m;
}


static sf::IntRect GetTileRange(
	const sf::FloatRect& bounds,
	int cellSize
) {
	const float left = bounds.position.x;
	const float top = bounds.position.y;
	const float right = bounds.position.x + bounds.size.x;
	const float bottom = bounds.position.y + bounds.size.y;

	const int startX = static_cast<int>(std::floor(left / cellSize));
	const int startY = static_cast<int>(std::floor(top / cellSize));

	const int endX = static_cast<int>(std::floor((right - 1.f) / cellSize));
	const int endY = static_cast<int>(std::floor((bottom - 1.f) / cellSize));

	return {
		{startX,startY},
		{endX - startX + 1, endY - startY + 1 }
	};
}

Manifold Collider::BoxVsTilemapManifold(const BoxCollider& box, const TileMapCollider& tilemapC) {

	Manifold best;
	const auto Bounds = sf::FloatRect(box.GetWorldPosition(), box.GetSize());
	const auto tilemap = tilemapC.getTileMap();
	const auto& layer = tilemap->getCollisionLayer(tilemapC.layerName);
	const int cellSize = layer.cellSize;
	
	auto range = GetTileRange(Bounds, cellSize);

	for (int y = range.position.y; y < range.position.y + range.size.y; ++y) {
		for (int x = range.position.x; x < range.position.x + range.size.x; ++x) {
			if (!tilemap->isSolidTile(layer,x, y))
				continue;

			BoxCollider tile;
			tile._backupPosition = {
				float(x * cellSize),
				float(y * cellSize)
				};
			tile._size = { (float)cellSize, (float)cellSize };
			tile._halfSize = { cellSize / 2.0f, cellSize / 2.0f };

			Manifold m = BoxVsBoxManifold(box, tile);
			if (!m.hit) continue;

			if (!best.hit || m.penetration > best.penetration)
				best = m;
		}
	}
	return best;

}

Manifold Collider::GetManifold(const Collider* a, const Collider* b)
{
	
	switch (a->type)
	{
		case ColliderType::Box:
			switch (b->type)
			{
				case ColliderType::Box:
					return BoxVsBoxManifold(
						*(const BoxCollider*)a,
						*(const BoxCollider*)b
					);

				case ColliderType::Circle:
					return BoxVsCircleManifold(
						*(const BoxCollider*)a,
						*(const CircleCollider*)b
					);

				case ColliderType::Tilemap:
					return BoxVsTilemapManifold(
						*(const BoxCollider*)a,
						*(const TileMapCollider*)b
					);


			}
			break;

		case ColliderType::Circle:
			switch (b->type)
			{
				case ColliderType::Box:
					{
						Manifold m = BoxVsCircleManifold(
							*(const BoxCollider*)b,
							*(const CircleCollider*)a
						);
						m.normal = -m.normal; // important
						return m;
					}

				case ColliderType::Circle:
					return CircleVsCircleManifold(
						*(const CircleCollider*)a,
						*(const CircleCollider*)b
					);


				case ColliderType::Tilemap:
					return CircleVsTilemapManifold(
						*(const CircleCollider*)a,
						*(const TileMapCollider*)b
					);
			}
			break;

	}
	return {};
}



bool Collider::CheckPoint(const sf::Vector2f& p, const Collider* col)
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

bool Collider::CheckCollision(const Collider* a, const Collider* b) {
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


void Collider::OnCollisionEnter(const Collider* other) {
	Manifold m = Collider::GetManifold(this, other);
	_onCollisionEnter(m, other);
}

void Collider::OnCollisionExit(const Collider* other) {
	Manifold m = Collider::GetManifold(this, other);
	_onCollisionExit(m, other);
}

void Collider::OnCollisionStay(const Collider* other) {
	Manifold m = Collider::GetManifold(this, other);
	_onCollisionStay(m, other);
}