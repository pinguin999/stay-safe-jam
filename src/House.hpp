#pragma once

#include "GameObject.hpp"

#include <Box2D/Box2D.h>
#include <jngl.hpp>

class House : public GameObject {
public:
	House(b2World&, jngl::Vec2 position);
	bool step() override;
	void draw() const override;

	void onContact(GameObject*) override;

private:
	jngl::Sprite sprite{"house"};
};
