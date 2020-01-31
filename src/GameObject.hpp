#pragma once

#include <jngl/Vec2.hpp>

class b2Body;
class b2Shape;

/// Basisklasse für die physischen Objekte im Spiel
class GameObject {
public:
	virtual bool step() = 0;
	virtual void draw(bool transparent, float scale) const = 0;
	virtual void createFixture() = 0;

	jngl::Vec2 getPosition() const;
	void setPosition(jngl::Vec2 position);

	float getRotation() const;
	void setRotation(float radian);

	virtual int getPrice() const = 0;

	virtual ~GameObject() = default;

	virtual int getAmount() const;

protected:
	void createFixtureFromShape(const b2Shape&);

	b2Body* body;
};
