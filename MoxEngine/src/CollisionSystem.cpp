#include "CollisionSystem.h"
#include "GameObject.h"

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

    // 1. Clear current collisions
    for (Collider* c : _Colliders)
        c->_curCollisions.clear();

    // 2. Collect all collisions for this frame
    for (size_t i = 0; i < count; ++i) {
        Collider* a = _Colliders[i];

        for (size_t j = i + 1; j < count; ++j) {
            Collider* b = _Colliders[j];

            if (Collider::CheckCollision(a, b)) {
                a->_curCollisions.push_back(b);
                b->_curCollisions.push_back(a);
            }
        }
    }

    // 3. Compare previous vs current
    for (Collider* c : _Colliders)
    {
        // oncollisionenter
        for (Collider* cur : c->_curCollisions) {
            if (std::find(c->_prevCollisions.begin(), c->_prevCollisions.end(), cur)
                == c->_prevCollisions.end())
            {
                c->OnCollisionEnter(cur);
            }
            else {
                c->OnCollisionStay(cur);
            }
        }

        // oncollisionexit
        for (Collider* prev : c->_prevCollisions) {
            if (std::find(c->_curCollisions.begin(), c->_curCollisions.end(), prev)
                == c->_curCollisions.end())
            {
                c->OnCollisionExit(prev);
            }
        }

        // 4. Prepare for next frame
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