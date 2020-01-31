#include "Text.hpp"

#include <jngl/font.hpp>

Text::Text(const std::string& str, jngl::Vec2 position) : Sprite(position), text(str) {
	static auto font = std::make_shared<jngl::Font>("Lato-Regular.ttf", 35);
	text.setFont(*font);
	text.setCenter(jngl::Vec2(0, 0));
}

void Text::drawSelf() const {
	text.draw();
}
