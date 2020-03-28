#pragma once

#include <jngl/Vec2.hpp>

class Control {
public:
	virtual ~Control() = default;

	/// Richtungsvektor
	virtual jngl::Vec2 getMovement() const = 0;
	virtual bool dash() const = 0;
    virtual bool punsh() const = 0;
    virtual bool shoot() const = 0;
    virtual bool shield() const = 0;

	virtual void vibrate();
};
