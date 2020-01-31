#include "GameObject.hpp"

#include "constants.hpp"

#include <Box2D/Box2D.h>

jngl::Vec2 GameObject::getPosition() const {
	return meterToPixel(body->GetPosition());
}

void GameObject::setPosition(const jngl::Vec2 position) {
	const auto transform = body->GetTransform();
	body->SetTransform(pixelToMeter(position), transform.q.GetAngle());
}

void GameObject::setRotation(const float radian) {
	const auto transform = body->GetTransform();
	body->SetTransform(transform.p, radian);
}

float GameObject::getRotation() const {
	return body->GetTransform().q.GetAngle();
}

int GameObject::getAmount() const {
	return 0;
}

void GameObject::createFixtureFromShape(const b2Shape& shape) {
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.7f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.filter.categoryBits = FILTER_CATEGORY_SOLID_OBJECT;
	fixtureDef.filter.maskBits = 0xffff;
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(1);
	body->ApplyLinearImpulseToCenter({ 0, -2 * body->GetMass() }, true);
}
