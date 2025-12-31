#include "Collider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "TilemapCollider.h"
#include "TilemapComponent.h"
#include "TileMap.h"
#include "GameObject.h"
#include "CollisionSystem.h"

ManifoldFn Collider::collisionDispatch[3][3] = {
	/* Box */
	{
		&Collider::BoxBox,
		&Collider::BoxCircle,
		&Collider::BoxTilemap
	},
	/* Circle */
	{
		&Collider::CircleBox,
		&Collider::CircleCircle,
		&Collider::CircleTilemap
	},
	/* Tilemap */
	{
		nullptr,
		nullptr,
		nullptr
	}
};



Manifold Collider::Collide(const Collider& a, const Collider& b) {
	if (b._isTrigger) return Manifold{};
	auto fn = collisionDispatch[(int)a.type][(int)b.type];
	return fn ? fn(a, b) : Manifold{};
}




Manifold Collider::BoxBox(const Collider& a, const Collider& b)
{
	return BoxVsBoxManifold(
		static_cast<const BoxCollider&>(a),
		static_cast<const BoxCollider&>(b)
	);
}

Manifold Collider::BoxCircle(const Collider& a, const Collider& b)
{
	return BoxVsCircleManifold(
		static_cast<const BoxCollider&>(a),
		static_cast<const CircleCollider&>(b)
	);
}

Manifold Collider::CircleBox(const Collider& a, const Collider& b)
{
	Manifold m = BoxVsCircleManifold(
		static_cast<const BoxCollider&>(b),
		static_cast<const CircleCollider&>(a)
	);
	m.normal = -m.normal;
	return m;
}

Manifold Collider::CircleCircle(const Collider& a, const Collider& b)
{
	return CircleVsCircleManifold(
		static_cast<const CircleCollider&>(a),
		static_cast<const CircleCollider&>(b)
	);
}

Manifold Collider::BoxTilemap(const Collider& a, const Collider& b)
{
	return BoxVsTilemapManifold(
		static_cast<const BoxCollider&>(a),
		static_cast<const TileMapCollider&>(b)
	);
}

Manifold Collider::CircleTilemap(const Collider& a, const Collider& b)
{
	return CircleVsTilemapManifold(
		static_cast<const CircleCollider&>(a),
		static_cast<const TileMapCollider&>(b)
	);
}


sf::Vector2f Collider::GetWorldPosition() const {

	if (!_transform) return _backupPosition;// +offset;
	sf::Vector2f scale = _transform->GetScale();
	sf::Vector2f offset{ _colliderOrigin.x * scale.x, _colliderOrigin.y * scale.y };
	
	return _transform->GetPosition() + offset;
}


Manifold Collider::BoxVsBoxManifold(const BoxCollider& a, const BoxCollider& b)
{
	sf::Vector2f aMin = a.GetWorldPosition();
	sf::Vector2f aMax = aMin + a.GetSize();

	sf::Vector2f bMin = b.GetWorldPosition();
	sf::Vector2f bMax = bMin + b.GetSize();

	float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
	float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

	if (overlapX < -EPSILON || overlapY < -EPSILON)
		return {};

	Manifold m;
	m.hit = true;

	if (overlapX < overlapY)
	{
		m.penetration = std::max(overlapX, 0.f);
		m.normal = (aMin.x < bMin.x) ? sf::Vector2f{ -1.f, 0.f }
		: sf::Vector2f{ 1.f, 0.f };
	}
	else
	{
		m.penetration = std::max(overlapY, 0.f);
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


	return {};
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

Manifold Collider::BoxVsTilemapManifold(const BoxCollider& box, const TileMapCollider& tilemapCol) {

	Manifold best;
	const auto Bounds = sf::FloatRect(box.GetWorldPosition(), box.GetSize());
	const auto tilemapComp= tilemapCol.getTileMapComponent();

	const auto tilemap = tilemapComp->GetTilemap();
	const auto& layer = *tilemap->getCollisionLayer(tilemapComp->GetLayerName());
	const int cellSize = layer.cellSize;
	
	auto range = GetTileRange(Bounds, cellSize);

	for (int y = range.position.y; y < range.position.y + range.size.y; ++y) {
		for (int x = range.position.x; x < range.position.x + range.size.x; ++x) {
			if (!tilemap->isSolidTile(layer, x, y)) continue;


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
	return Collide(*a, *b);
}


bool Collider::CheckCollision(const Collider* a, const Collider* b) {
	return Collide(*a, *b).hit;
}


void Collider::OnCollisionEnter(const Collider* other, const Manifold& m) {
	_onCollisionEnter(m, other);
}

void Collider::OnCollisionExit(const Collider* other) {
	_onCollisionExit(other);
}

void Collider::OnCollisionStay(const Collider* other, const Manifold& m) {
	_onCollisionStay(m, other);
}