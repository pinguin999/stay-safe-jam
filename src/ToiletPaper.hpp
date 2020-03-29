#include "GameObject.hpp"
#include <box2d/box2d.h>
#include <jngl.hpp>

class ToiletPaper : public GameObject {
public:
	ToiletPaper(b2World& world, jngl::Vec2 position);

	void draw() const override;
	bool step() override;
    void onContact(GameObject*) override;
    
private:
	jngl::Sprite sprite{"tp"};
};