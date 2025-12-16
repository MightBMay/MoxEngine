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
    static inline const float maxStepDistance = 4.f;
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


    static void MoveKinematic(
        Transform& transform,
        Collider* collider, 
        const sf::Vector2f& delta
    );

    static void MoveAxis(Transform& transform, Collider* collider, float amount, bool isXAxis = true);

};