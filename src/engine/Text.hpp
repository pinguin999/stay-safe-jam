#pragma once

#include "Sprite.hpp"

#include <jngl/text.hpp>

class Text : public Sprite {
public:
	Text(const std::string& str, jngl::Vec2 position);

protected:
	void drawSelf() const;

private:
	jngl::Text text;
};
