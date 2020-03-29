#include "Game.hpp"

#include "constants.hpp"
#include "engine/Animation.hpp"
#include "engine/Fade.hpp"
#include "engine/Sprite.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "House.hpp"
#include "ToiletPaper.hpp"

#include <cmath>
#include <jngl.hpp>

Game::Game()
: world({ 0, 0 }),
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

	int streets[4] = {-330, -100, 100, 330};
	for(int i = 0; i < 4; i++) {
		// oben rechts
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(-650, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(-550, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(-450, streets[i])));

		// oben mitte
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(-200, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(-100, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(0, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(100, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(200, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(300, streets[i])));
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(400, streets[i])));

		// oben rechts
		gameObjects.emplace_back(std::make_shared<House>(world, jngl::Vec2(650, streets[i])));
	}

	gameObjects.emplace_back(std::make_shared<Player>(world, jngl::Vec2(-300, 300), 1));
	gameObjects.emplace_back(std::make_shared<ToiletPaper>(world, jngl::Vec2(-250, 250)));

}

Game::~Game() = default;

void Game::onLoad() {
	jngl::setMouseVisible(false);
}

void Game::step() {
	world.Step((1.f / 60.f), 8, 3);

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
	background.draw();
	for (auto& gameObject : gameObjects) {
		gameObject->draw();
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
