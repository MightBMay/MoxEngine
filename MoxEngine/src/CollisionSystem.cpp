#include "CollisionSystem.h"
#include "GameObject.h"
#include "RigidBody.h"
void CollisionSystem::RegisterContact(Collider* a, Collider* b, const Manifold& m) {
   // _Contacts.emplace(MakeContact(a, b), m);
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
    

    for (int i = 0; i < count ; i++) {
        for (int j = i + 1; j < count; j++) {

            Collider* a = _Colliders[i];
            Collider* b = _Colliders[j];
            Manifold m = Collider::GetManifold(a,b);
            if (m.hit) _Contacts.emplace(MakeContact(a, b), m);

        }

    }

#pragma region Collision Events



#pragma region OnCollision Enter/Stay



    for (auto& [key, manifold] : _Contacts) {

        auto it = _PrevContacts.find(key);

        Collider* a = key.a;
        Collider* b = key.b;

        if (it == _PrevContacts.end()) {
            a->OnCollisionEnter(b, manifold);

            Manifold flipped = manifold;
            flipped.normal = -flipped.normal;
            b->OnCollisionEnter(a, flipped);
        }
        else{
            a->OnCollisionEnter(b, manifold);

            Manifold flipped = manifold;
            flipped.normal = -flipped.normal;
            b->OnCollisionEnter(a, flipped);
        }

    }
#pragma endregion
#pragma region OnCollision Exit



    for (auto& [ key, _ ] : _PrevContacts) {
        if (_Contacts.find(key) == _Contacts.end()) {
            Collider* a = key.a;
            Collider* b = key.b;

            a->OnCollisionExit(b);
            b->OnCollisionExit(a);
        }
    }
#pragma endregion

#pragma endregion


#pragma region Collision Resolution




    for (auto& [contact, manifold] : _Contacts) {


        Collider* a = contact.a;
        Collider* b = contact.b;

        RigidBody* aRB = a->_parent->GetComponent<RigidBody>();
        bool aStatic = true;
        if (aRB) aStatic = aRB->isKinematic;

        RigidBody* bRB = b->_parent->GetComponent<RigidBody>();
        bool bStatic = true;
        if (bRB) bStatic = bRB->isKinematic;


        float mass = 1; // unused rn.
        float aMoveFactor = 0.5f; // default- movement split 50/50 .

        if (aStatic && !bStatic) {
            aMoveFactor = 0;
        }
        else if (!aStatic && bStatic) {
            aMoveFactor = 1;
        }
        

        sf::Vector2f separation = manifold.normal * manifold.penetration;

        a->_parent->_transform->Move(-separation * aMoveFactor);
        b->_parent->_transform->Move(separation * (1.0f - aMoveFactor));
    
        if (aRB) aRB->CollisionCancelVelocity(manifold, b);
        if (bRB) bRB->CollisionCancelVelocity(manifold, a);
    }







#pragma endregion





    _PrevContacts.swap(_Contacts);
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
    transform.Move(isXAxis ? sf::Vector2f{ amount, 0.f } : sf::Vector2f{ 0.f, amount });

    //for (Collider* other : CollisionSystem::GetColliders())
    //{
    //    if (other == collider)//|| other->isTrigger)
    //        continue;

    //    Manifold m = Collider::GetManifold(collider, other);
    //    if (!m.hit)
    //        continue;

    //    // Determine push direction
    //    float sign = (amount > 0.f) ? -1.f : 1.f;

    //    if (isXAxis)
    //    {

    //        transform.Move({ sign * m.penetration, 0.f });
    //    }
    //    else
    //    {
    //        transform.Move({ 0.f, sign * m.penetration });
    //    }
    //    auto rb = collider->_parent->GetComponent<RigidBody>();
    //    if (rb) rb->CollisionCancelVelocity(m, other);
    //
    //}
}
