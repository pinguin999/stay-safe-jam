#pragma once

#include <string>
#include <jngl/Vec2.hpp>

class Draw;

class Animation {
public:
	Animation(const std::string& foldername, bool repeat = true);
	Animation(const Animation&) = delete;
	void draw() const;

	/// Gibt true zurück, wenn sie entfernt werden soll
	bool step();

	bool Finished();

	/// Springt direkt zu einem bestimmten Frame
	void setFrame(unsigned int);

	void setPosition(jngl::Vec2);

	void reset();

private:
	std::string foldername_;
	int stepsPerFrame_;
	bool finished_;
	unsigned int numberOfFrames = 0;
	unsigned int currentFrame = 0;
	int stepsLeft_;
	std::string CreateFilename(int) const;
	bool repeat;
	jngl::Vec2 position;
};
