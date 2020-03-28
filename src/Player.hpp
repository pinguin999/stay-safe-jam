#pragma once

#include "GameObject.hpp"
#include "engine/Animation.hpp"

#include <array>
#include <Box2D/Box2D.h>
#include <jngl.hpp>
#include <memory>

class b2World;
class Control;

class Player : public GameObject {
public:
	Player(b2World& world, const jngl::Vec2 position, const int playerNr);
	~Player();

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	bool step() override;

	void dash(jngl::Vec2);
	void punsh();
	void shoot();
	void shield();

	void draw() const override;

	/// Zeichnet den Controller des Spielers mit den eingesammelten Fragments
	void drawHUD() const;
	void playerContactHUD(jngl::Vec2 position);

	void onContact(GameObject*) override;

	void createFixtureFromShape(const b2Shape& shape);
	bool isAlive() const;

	/// Der Spieler hat alles eingesammelt und kann jetzt töten
	bool isKing() const;
	void setKing(bool);

	void vibrate();

	int shield_up_time = 0;
	int points = 0;

private:
	jngl::Sprite sprite;
	jngl::Sprite spriteStunned;
	jngl::Sprite shadow;
	jngl::Sprite idle;
	jngl::Sprite crown;
	Animation walk;
	Animation animation_shield;
	Animation animation_attack;
	bool king = false;

	jngl::Sprite gamepad{"gamepad"};

	const int playerNr;

	// wird ungleichmäßig hochgezählt für die Animation des Kopfs
	float time = 0;

	int dashCountdown = 0;
	b2Vec2 dashDirection;

	int stun_time = 0;
	bool shield_active = false;
	bool has_punched = false;

	std::unique_ptr<Control> control;

	jngl::Vec2 hudPosition;
	bool hudCollision = false;

	bool alive = true;
};
