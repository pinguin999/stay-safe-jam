#pragma once

#include <Box2D/Box2D.h>
#include <jngl/work.hpp>
#include <set>
#include <vector>
#include <jngl.hpp>

class Animation;
class GameObject;
class Sprite;

class Game : public jngl::Work {
public:
	Game();
	~Game();
	void onLoad() override;
	void step() override;
	void draw() const override;
	void addSprite(Sprite*);
	void removeSprite(Sprite*);

	/// Böse!
	b2World& getWorld();

private:
	b2World world;

	/// Alle gebauten Blöcke
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	std::vector<std::unique_ptr<Animation>> animations;

	std::vector<std::unique_ptr<Sprite>> sprites;
	std::set<Sprite*> needToRemove;

	b2Body* ground;
	const b2Vec2 groundPosition;
	jngl::Sprite background{"background"};
};
