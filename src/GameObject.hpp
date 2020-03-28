#pragma once

#include <jngl/Vec2.hpp>

class b2Body;
class b2Shape;

/// Basisklasse für die physischen Objekte im Spiel
class GameObject {
public:
	/// true zurückgeben damit das Objekt entfernt wird
	virtual bool step() = 0;

	virtual void draw() const = 0;

	virtual jngl::Vec2 getPosition() const;
	void setPosition(jngl::Vec2 position);

	float getRotation() const;
	void setRotation(float radian);

	virtual ~GameObject() = default;

	virtual int getAmount() const;

	virtual double getZIndex() const;

	virtual void onContact(GameObject* other);

	void checkOutOfScreen();

protected:
	void createFixtureFromShape(const b2Shape&);

	b2Body* body;
};
