#pragma once
#include "pch.h"
class Transform {


private:
	sf::Transformable _transformable;





public:
	Transform() = default;

	void SetPosition(const sf::Vector2f& position) {_transformable.setPosition(position);}

    void SetRotation(float degrees) { _transformable.setRotation(sf::degrees(degrees)); }
    void SetRotationRad(float radians) { _transformable.setRotation(sf::radians(radians)); }

    void SetRotationRaw(sf::Angle rotation) {_transformable.setRotation(rotation); }

    void SetScale(const sf::Vector2f& scale) { _transformable.setScale(scale); }

    void Move(const sf::Vector2f& amount) {
        sf::Vector2f newPosition = _transformable.getPosition() + amount;
        SetPosition(newPosition);
    }

    void Move(const sf::Vector2f& direction, const float speed) {
        sf::Vector2f newPosition = _transformable.getPosition() + (direction*speed);
        SetPosition(newPosition);
    }


    const sf::Vector2f& GetPosition() const { return _transformable.getPosition(); }
    float GetRotationDeg() const { return _transformable.getRotation().asDegrees(); }
    float GetRotationRad() const { return _transformable.getRotation().asRadians(); }
    sf::Angle GetRotationRaw() const { return _transformable.getRotation(); }
    const sf::Vector2f& GetScale() const { return _transformable.getScale(); }

    const sf::Transform& GetSFTransform() const { return _transformable.getTransform(); }





};