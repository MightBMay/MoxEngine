#include "CollisionSystem.h"
#include "GameObject.h"
#include "RigidBody.h"
void CollisionSystem::RegisterContact(Collider* a, Collider* b, const Manifold& m) {
    _Contacts.insert({a,b, m});
}

void CollisionSystem::AddCollider(Collider* col) {
    _Colliders.push_back(col);
}

void CollisionSystem::RemoveCollider(Collider* col) {
    auto it = std::find(_Colliders.begin(), _Colliders.end(), col);
    if (it != _Colliders.end()) {
        std::swap(*it, _Colliders.back());
        _Colliders.pop_back();
    }
}

void CollisionSystem::Update(float deltaTime)
{
    // 1) Clear per-frame collision lists
    for (Collider* c : _Colliders)
        c->_curCollisions.clear();

    _Contacts.clear();

    // 2) Broad + narrow phase: build contacts ONLY
    for (size_t i = 0; i < _Colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < _Colliders.size(); ++j)
        {
            Collider* a = _Colliders[i];
            Collider* b = _Colliders[j];

            Manifold m = Collider::GetManifold(a, b);
            if (!m.hit)
                continue;

            RegisterContact(a, b, m);
        }
    }

    // 3) Resolve contacts
    for (const Contact& contact : _Contacts)
    {
        Collider* a = contact.a;
        Collider* b = contact.b;
        const Manifold& m = contact.m;

        // Simple positional correction (static assumed for now)
        Transform* ta = a->_parent->_transform.get();
        Transform* tb = b->_parent->_transform.get();

        // Push A out of B (or vice versa — adjust as needed)
        ta->Move(m.normal * m.penetration);
    }

    // 4) Build current collision sets + fire Enter/Stay
    for (const Contact& contact : _Contacts)
    {
        Collider* a = contact.a;
        Collider* b = contact.b;
        const Manifold& m = contact.m;

        a->_curCollisions.push_back(b);
        b->_curCollisions.push_back(a);

        // --- A side ---
        if (std::find(a->_prevCollisions.begin(),
            a->_prevCollisions.end(), b)
            == a->_prevCollisions.end())
        {
            a->OnCollisionEnter(b, m);
        }
        else
        {
            a->OnCollisionStay(b, m);
        }

        // --- B side ---
        Manifold flipped = m;
        flipped.normal = -flipped.normal;

        if (std::find(b->_prevCollisions.begin(),
            b->_prevCollisions.end(), a)
            == b->_prevCollisions.end())
        {
            b->OnCollisionEnter(a, flipped);
        }
        else
        {
            b->OnCollisionStay(a, flipped);
        }
    }

    // 5) Fire Exit events
    for (Collider* c : _Colliders)
    {
        for (Collider* prev : c->_prevCollisions)
        {
            if (std::find(c->_curCollisions.begin(),
                c->_curCollisions.end(), prev)
                == c->_curCollisions.end())
            {
                c->OnCollisionExit(prev);
            }
        }

        c->_prevCollisions.swap(c->_curCollisions);
    }
}

void CollisionSystem::ResolveCollisions(GameObject& obj, bool isXAxis) {

    Collider* self = obj.getCollider();
    if (!self) return;

    for (Collider* other : _Colliders)
    {
        if (other == self)
            continue;

        //if (other->isTrigger)
           // continue;

        Manifold m = Collider::GetManifold(self, other);
        if (!m.hit)
            continue;

        // Axis filtering
        if (isXAxis)
        {
            if (std::abs(m.normal.x) < 0.9f)
                continue;
        }
        else
        {
            if (std::abs(m.normal.y) < 0.9f)
                continue;
        }
        // Positional correction
        obj._transform->Move(m.normal * m.penetration);

        // Stop after first solid correction
        break;
    }

}


void CollisionSystem::MoveKinematic(
    Transform& transform,
    Collider* collider,
    const sf::Vector2f& delta
) {
    float dist = std::hypot(delta.x, delta.y);
    if (dist == 0.f) return;

    int steps = std::ceil(dist / maxStepDistance);
    sf::Vector2f step = delta / (float)steps;

    for (int i = 0; i < steps; ++i)
    {
        if(step.x != 0)
            MoveAxis(transform, collider, step.x, true);

        if(step.y != 0) // don't move if step has 0 y movement
            MoveAxis(transform, collider, step.y, false);
    }
}


void CollisionSystem::MoveAxis(Transform& transform, Collider* collider, float amount, bool isXAxis)
{
    // Try full move
    transform.Move(isXAxis ? sf::Vector2f{ amount, 0.f }
    : sf::Vector2f{ 0.f, amount });
    
}
