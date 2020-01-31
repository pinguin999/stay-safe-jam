#include "Fade.hpp"

#include <boost/math/constants/constants.hpp>
#include <jngl.hpp>

Fade::Fade(std::shared_ptr<Work> work, int speed)
: work_(std::move(work)), oldWork_(jngl::getWork()), fadeCount_(0), speed_(speed) {
}

void Fade::step() {
	const int maxAlpha = 255;
	fadeCount_ += speed_;
	if (fadeCount_ >= 2 * maxAlpha) { // Finished?
		jngl::setWork(work_);
	}
}

void Fade::draw() const {
	const int maxAlpha = 255;
	if (fadeCount_ < maxAlpha) {
		if (!dynamic_cast<Fade*>(oldWork_.get())) {
			jngl::scale(std::sin((510 - fadeCount_) / 510 * boost::math::constants::pi<double>() * 0.5));
			oldWork_->draw();
		}
	} else {
		jngl::scale(std::sin(fadeCount_ / 510 * boost::math::constants::pi<double>() * 0.5));
		work_->draw();
	}
	const int alpha = static_cast<int>(fadeCount_ > maxAlpha ? 2 * maxAlpha - fadeCount_ : fadeCount_);
	jngl::setColor(0x3c, 0x86, 0xe6, alpha);
	jngl::drawRect(-jngl::getScreenWidth() / 2, -jngl::getScreenHeight() / 2,
	               jngl::getScreenWidth(), jngl::getScreenHeight());
	jngl::setColor(255, 255, 255, 255);
}
