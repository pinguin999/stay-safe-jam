#include "constants.hpp"
#include "engine/Options.hpp"
#include "init.hpp"

#include <jngl/App.hpp>
#include <jngl/main.hpp>
#include <jngl/message.hpp>
#include <jngl/input.hpp>
#include <jngl/job.hpp>

class QuitWithEscape : public jngl::Job {
public:
	void step() override {
		if (jngl::keyPressed(jngl::key::Escape)) {
			jngl::quit();
		}
	}
	void draw() const override {
	}
};

JNGL_MAIN_BEGIN {
	jngl::App app(programDisplayName);
	if (init()) {
#ifdef NDEBUG
		try {
#endif
			jngl::addJob(std::make_shared<QuitWithEscape>());
			app.mainLoop();
			getOptions().Save();
#ifdef NDEBUG
		} catch (std::exception& e) {
			jngl::errorMessage(e.what());
		}
#endif
	}
} JNGL_MAIN_END
