#include "Game.hpp"

#include "constants.hpp"
#include "engine/Animation.hpp"
#include "engine/Fade.hpp"
#include "engine/Sprite.hpp"
#include "GameObject.hpp"

#include <cmath>
#include <jngl.hpp>

Game::Game()
: world({ 0, 9.8 /* gravity */ }),
  groundPosition(pixelToMeter({ 0, GROUND + PIXEL_PER_METER })) {

	b2BodyDef bodyDef;
	bodyDef.position = groundPosition;
	bodyDef.type = b2_kinematicBody;
	ground = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(BOUNDS_W / PIXEL_PER_METER, 1);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.7f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.filter.categoryBits = FILTER_CATEGORY_SOLID_OBJECT;
	fixtureDef.filter.maskBits = 0xffff;
	ground->CreateFixture(&fixtureDef);
}

Game::~Game() = default;

void Game::onLoad() {
	jngl::setMouseVisible(false);
}

void Game::step() {

	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {
		if ((*it)->step() || (*it)->getPosition().y > 1000) {
			it = gameObjects.erase(it);
			if (it == gameObjects.end()) {
				break;
			}
		}
	}
	for (auto it = animations.begin(); it != animations.end(); ++it) {
		if ((*it)->step()) {
			it = animations.erase(it);
			if (it == animations.end()) {
				break;
			}
		}
	}
	for (auto& sprite : sprites) {
		sprite->step();
	}
	for (auto& sprite : needToRemove) {
		sprites.erase(std::remove_if(
		                  sprites.begin(), sprites.end(),
		                  [sprite](const std::unique_ptr<Sprite>& p) { return p.get() == sprite; }),
		              sprites.end());
	}
	needToRemove.clear();
}

void Game::draw() const {
	for (auto& gameObject : gameObjects) {
		gameObject->draw(false, 1);
	}
	for (auto& animation : animations) {
		animation->draw();
	}
	for (auto& sprite : sprites) {
		sprite->draw();
	}
}

b2World& Game::getWorld() {
	return world;
}

void Game::addSprite(Sprite* sprite) {
	sprites.emplace_back(sprite);
}

void Game::removeSprite(Sprite* sprite) {
	needToRemove.insert(sprite);
}