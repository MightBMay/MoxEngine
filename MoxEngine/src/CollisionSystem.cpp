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
    const size_t count = _Colliders.size();

    for (Collider* c : _Colliders)
        c->_curCollisions.clear();

    for (const Contact& contact : _Contacts) {
        Collider* a = contact.a;
        Collider* b = contact.b;
        const Manifold& m = contact.m;

        if (std::find(
            a->_curCollisions.begin(),
            a->_curCollisions.end(), b) 
            == a->_curCollisions.end()) {
            a->_curCollisions.push_back(b);
            b->_curCollisions.push_back(a);
        }


        // invoke events for A
        if (std::find(a->_prevCollisions.begin(), a->_prevCollisions.end(), b) == a->_prevCollisions.end())
        {
            a->OnCollisionEnter(b, m); // pass stored manifold
        }
        else
        {
            a->OnCollisionStay(b, m); // pass stored manifold
        }

        // invoke events for B
        if (std::find(b->_prevCollisions.begin(), b->_prevCollisions.end(), a) == b->_prevCollisions.end())
        {
            b->OnCollisionEnter(a, m);
        }
        else
        {
            b->OnCollisionStay(a, m);
        }

    }
    // handle / invoke exit event.
    for (Collider* c : _Colliders) {
        for (Collider* prev : c->_prevCollisions) {
            if (std::find(c->_curCollisions.begin(), c->_curCollisions.end(), prev) == c->_curCollisions.end())
                c->OnCollisionExit(prev);
        }

        c->_prevCollisions.swap(c->_curCollisions);
    }


 
    _Contacts.clear();












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

    for (Collider* other : CollisionSystem::GetColliders())
    {
        if (other == collider)//|| other->isTrigger)
            continue;

        Manifold m = Collider::GetManifold(collider, other);
        if (!m.hit)
            continue;

        CollisionSystem::RegisterContact(collider, other, m);
        // Determine push direction
        float sign = (amount > 0.f) ? -1.f : 1.f;

        if (isXAxis)
        {

            transform.Move({ sign * m.penetration, 0.f });
        }
        else
        {
            transform.Move({ 0.f, sign * m.penetration });
        }
        auto rb = collider->_parent->GetComponent<RigidBody>();
        if (rb) rb->CollisionCancelVelocity(m, other);
        break;
    }
}
