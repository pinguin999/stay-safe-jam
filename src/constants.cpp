#include "constants.hpp"

#include <Box2D/Box2D.h>

jngl::Vec2 meterToPixel(const b2Vec2& v) {
	return {v.x * PIXEL_PER_METER, v.y * PIXEL_PER_METER};
}

b2Vec2 pixelToMeter(const jngl::Vec2& v) {
	return b2Vec2(v.x / PIXEL_PER_METER, v.y / PIXEL_PER_METER);
}
