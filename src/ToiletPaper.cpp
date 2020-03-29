#include "ToiletPaper.hpp"
#include "constants.hpp"
#include "Player.hpp"

ToiletPaper::ToiletPaper(b2World& world, const jngl::Vec2 position) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = pixelToMeter(position);
	body = world.CreateBody(&bodyDef);
	body->SetLinearDamping(10.f);
	body->SetAngularDamping(10);

	b2CircleShape shape = b2CircleShape();
	shape.m_radius = pixelToMeter(25);
	b2FixtureDef fixtureDef;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.7f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.filter.categoryBits = FILTER_CATEGORY_SOLID_OBJECT;
	fixtureDef.filter.maskBits = 0xffff;
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(1);
    createFixtureFromShape(shape);

}

bool ToiletPaper::step() {
	checkOutOfScreen();
	return false;
}

void ToiletPaper::draw() const {
	jngl::pushMatrix();
	const auto transform = body->GetTransform();
	jngl::translate(meterToPixel(transform.p));
	jngl::rotate(transform.q.GetAngle() * 180 / M_PI);
	sprite.draw();
	jngl::popMatrix();
}

void ToiletPaper::onContact(GameObject* other){
	if (const auto player = dynamic_cast<Player*>(other)) {
		body->GetWorld()->DestroyBody(body);
	}
}

