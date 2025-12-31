#pragma once
#include <vector>
#include <algorithm>
#include "Collider.h"
#include <unordered_set>




struct Manifold {
    bool hit = false;
    sf::Vector2f normal{ 0,0 };
    float penetration = 0;
};

// contacts are done using a key/hash system with a set, as i was having issues with collision substeps registering the same collision
// multiple times in one frame.
struct Contact{
    Collider* a;
    Collider* b;
    Manifold m;

    bool operator==(const Contact& other) const { 
        return (a == other.a && b == other.b) ||
            (a == other.b && b == other.a);
    }
};

struct ContactKeyHash {
    size_t operator()(const Contact& k) const {
        return std::hash<Collider*>()(k.a) ^ std::hash<Collider*>()(k.b);
    }
};

class CollisionSystem {
private:
	static inline std::vector<Collider*> _Colliders;
    static inline std::unordered_set<Contact, ContactKeyHash> _Contacts;

    CollisionSystem() = default;
    static inline const float maxStepDistance = 4.f;
public:

    static const std::unordered_set<Contact, ContactKeyHash>& GetContacts() { return _Contacts; }
    static void ClearContacts() { _Contacts.clear(); }

    static void RegisterContact(Collider* a, Collider* b, const Manifold& m);

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