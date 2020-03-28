#pragma once

#include "Control.hpp"

class Keyboard : public Control {
public:
	Keyboard(int playerNr);

	jngl::Vec2 getMovement() const override;
	bool dash() const override;
	bool punsh() const override;
	bool shoot() const override;
	bool shield() const override;

private:
	const int playerNr;
};