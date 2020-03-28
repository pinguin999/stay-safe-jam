#pragma once

#include "Control.hpp"

#include <jngl/Controller.hpp>

class Gamepad : public Control {
public:
	Gamepad(std::shared_ptr<jngl::Controller>, int playerNr);
	jngl::Vec2 getMovement() const override;
	bool dash() const override;
    bool punsh() const override;
    bool shoot() const override;
    bool shield() const override;

	void vibrate() override;

private:
	std::shared_ptr<jngl::Controller> controller;
	const int playerNr;
};
