#include "House.hpp"
#include "constants.hpp"

House::House(b2World& world, const jngl::Vec2 position) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = pixelToMeter(position);
	body = world.CreateBody(&bodyDef);
	body->SetLinearDamping(10.f);
	body->SetAngularDamping(10);

	body->SetUserData(static_cast<GameObject*>(this));

	b2PolygonShape shape;
	shape.SetAsBox(64. / PIXEL_PER_METER / 2., 64. / PIXEL_PER_METER / 2.);
	createFixtureFromShape(shape);
}

bool House::step() {
	checkOutOfScreen();
	return false;
}

void House::draw() const {
	jngl::pushMatrix();
	const auto transform = body->GetTransform();
	jngl::translate(meterToPixel(transform.p));
	jngl::rotate(transform.q.GetAngle() * 180 / M_PI);
	sprite.draw();
	jngl::popMatrix();
}

void House::onContact(GameObject* other){
	if(const auto punch = dynamic_cast<House*>(other)) {
		b2Vec2 vec = body->GetPosition() - pixelToMeter(punch->getPosition());
		body->ApplyLinearImpulse(vec, body->GetPosition(), true);
	}
}
