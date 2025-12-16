#pragma once
#include <vector>
#include <algorithm>
#include "Collider.h"




struct Manifold {
    bool hit = false;
    sf::Vector2f normal{ 0,0 };
    float penetration = 0;
};

class CollisionSystem {
private:
	static inline std::vector<Collider*> _Colliders;

    CollisionSystem() = default;

public:

    static CollisionSystem instance() {
        static CollisionSystem inst;
        return inst;
    }

    static const std::vector<Collider*> GetColliders() { return _Colliders; }

    static void AddCollider(Collider* col);

    static void RemoveCollider(Collider* col);

    static void Update(float deltaTime);

    static void ResolveCollisions(GameObject& obj, bool isXAxis = true);

};