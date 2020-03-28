#include "Player.hpp"
#include "Gamepad.hpp"
#include "Keyboard.hpp"

#include "constants.hpp"
#include <cmath>

Player::Player(b2World& world, const jngl::Vec2 position, const int playerNr)
: playerNr(playerNr), sprite("car"),
  spriteStunned("player" + std::to_string(playerNr) + "_stunn"), shadow("shadow"),
  idle("body_idle"), walk("body_walk", true), animation_shield("shield", true),
  animation_attack("attack", false), shield_active(false), crown("crown") {
	b2BodyDef bodyDef;
	bodyDef.position = pixelToMeter(position);
	bodyDef.type = b2_dynamicBody;
	body = world.CreateBody(&bodyDef);
	body->SetGravityScale(0);
	body->SetUserData(static_cast<GameObject*>(this));
	body->SetLinearDamping(10.f);

	b2CircleShape shape = b2CircleShape();
	shape.m_radius = 12 / PIXEL_PER_METER;
	createFixtureFromShape(shape);

	const auto controllers = jngl::getConnectedControllers();
	if (controllers.size() > playerNr) {
		control = std::make_unique<Gamepad>(controllers[playerNr], playerNr);
	} else {
		control = std::make_unique<Keyboard>(playerNr);
	}

	switch (playerNr) {
	case 0:
		hudPosition = { 250, -145 };
		break;
	case 1:
		hudPosition = { 250, 145 };
		break;
	case 2:
		hudPosition = { -250, 145 };
		break;
	case 3:
		hudPosition = { -250, -145 };
		break;
	default:
		assert(false); // ungültige Spieler-Nr
	}

	shadow.setCenter(0, 10);
	crown.setCenter(0, -15);

}

Player::~Player() {
	if (body) {
		body->GetWorld()->DestroyBody(body);
	}
}

bool Player::step() {
	hudCollision = false;

	// is player stunned? skip step
	if (stun_time > 0) {
		stun_time--;
		return false;
	}
	if (!alive) {
		return false;
	}

	walk.step();
	animation_shield.step();
	if (has_punched) {
		animation_attack.step();
	}
	if (animation_attack.Finished()) {
		has_punched = false;
		animation_attack.reset();
	}

	checkOutOfScreen();
	jngl::Vec2 vec = control->getMovement() * 0.8;

	body->SetLinearVelocity(b2Vec2(vec.x, vec.y));

	time += float(90 + (rand() % 10)) / 1000.f;
	if (shield_up_time > 0) {
		shield_up_time--;
	} else {
		shield_active = false;
	}

	return false;
}

void Player::dash(const jngl::Vec2 vec) {
	dashDirection.x = vec.x * body->GetMass();
	dashDirection.y = vec.y * body->GetMass();
	dashCountdown = 15;
	jngl::play("sfx/dash.ogg");
}

void Player::shield() {
	jngl::play("sfx/shield.ogg");
	shield_active = true;
}

void Player::shoot() {
	jngl::play("sfx/shoot.ogg");
	b2Fixture fixture = body->GetFixtureList()[0];
	uint16 bit_mask = fixture.GetFilterData().categoryBits;
	jngl::Vec2 direction = control->getMovement();
	if (direction.x == 0 && direction.y == 0) {
		// we need to move to shoot
		return;
	}

}

void Player::draw() const {
	jngl::pushMatrix();
	jngl::translate(getPosition());
	shadow.draw();
	if (playerNr == 0) {
		jngl::setSpriteColor(51, 255, 51);
	} else if (playerNr == 1) {
		jngl::setSpriteColor(255, 0, 51);
	} else if (playerNr == 2) {
		jngl::setSpriteColor(51, 51, 255);
	} else if (playerNr == 3) {
		jngl::setSpriteColor(255, 255, 51);
	}
	if (!alive) {
		jngl::rotate(90);
	}
	{
		jngl::pushMatrix();
		jngl::translate(0, 6);
		if (alive && body->GetLinearVelocity().LengthSquared() > 0.1) {
			walk.draw();
		} else {
			idle.draw();
		}
		if (shield_active) {
			animation_shield.draw();
		}
		if (has_punched) {
			animation_attack.draw();
		}

		jngl::popMatrix();
	}
	jngl::Vec2 rotvec = control->getMovement();
	jngl::setSpriteColor(255, 255, 255);
	jngl::translate(-jngl::Vec2(0, 8 + (alive ? std::sin(time) : 0)));
	if (!alive || stun_time > 0) {
		spriteStunned.draw();
	} else {
		jngl::rotate(float(rotvec.x*-90));
		sprite.draw();
	}
	if (isKing())
	{
		crown.draw();
	}
	jngl::popMatrix();

}

void Player::playerContactHUD(jngl::Vec2 pos) {
	switch (playerNr) {
	case 0:
		if (pos.x > 175 && pos.y < -100) {
			hudCollision = true;
		}
		break;
	case 1:
		if (pos.x > 175 && pos.y > 100) {
			hudCollision = true;
		}
		break;
	case 2:
		if (pos.x < -175 && pos.y > 100) {
			hudCollision = true;
		}
		break;
	case 3:
		if (pos.x < -175 && pos.y < -100) {
			hudCollision = true;
		}
		break;
	default:
		break;
	}
}

void Player::drawHUD() const {

	jngl::pushMatrix();
	jngl::translate(hudPosition);
	if (hudCollision) {
		jngl::pushSpriteAlpha(150);
	} else {
		jngl::pushSpriteAlpha(230);
	}
	gamepad.draw();
	jngl::setFontSize(10);
	jngl::setFontColor(0_rgb);
	jngl::popSpriteAlpha();
	// jngl::print("fragments: " + std::to_string(fragments.size()), {-50, 0});
	jngl::popMatrix();
	jngl::pushSpriteAlpha(hudCollision ? 200 : 255);
	jngl::popSpriteAlpha();
}

void Player::onContact(GameObject* other) {
	if (!alive) {
		return;
	}
}

uint16 getFilterCategory(int playerNr) {
	switch (playerNr) {
	case 0:
		return FILTER_CATEGORY_PLAYER0;
	case 1:
		return FILTER_CATEGORY_PLAYER1;
	case 2:
		return FILTER_CATEGORY_PLAYER2;
	case 3:
		return FILTER_CATEGORY_PLAYER3;
	default:
		break;
	}
	return FILTER_CATEGORY_SOLID_OBJECT;
}

bool Player::isAlive() const {
	return alive;
}

bool Player::isKing() const {
	return king;
}

void Player::setKing(bool king) {
	this->king = king;
}

void Player::vibrate() {
	control->vibrate();
}

void Player::createFixtureFromShape(const b2Shape& shape) {
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.7f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.filter.categoryBits = getFilterCategory(playerNr);
	fixtureDef.filter.maskBits = 0xffff;
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(1);
}
