#pragma once

#include <jngl/Vec2.hpp>

struct b2Vec2;

const std::string programDisplayName = "GGJ2020";

enum {
	FILTER_CATEGORY_CANON_PLAYER0 = 0x01,
	FILTER_CATEGORY_SOLID_OBJECT = 0x02,
	FILTER_CATEGORY_NON_SOLID_OBJECT = 0x04,
	FILTER_CATEGORY_CANON_PLAYER1 = 0x08,
};

constexpr float PIXEL_PER_METER = 100.0f;
constexpr int GROUND = 378;
constexpr int BOUNDS_W = 960;
constexpr int BOUNDS_H = 540;

jngl::Vec2 meterToPixel(const b2Vec2&);
b2Vec2 pixelToMeter(const jngl::Vec2&);
