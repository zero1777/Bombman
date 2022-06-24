#ifndef ICEEFFECT_HPP
#define ICEEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

class IceEffect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	int damage;
	float timeSpan = 0.5;
	int ID;
	bool P1_getFreezed;
	bool P2_getFreezed;
public:
	IceEffect(std::string img, float x, float y, float w, float h, int id);
	void Update(float deltaTime) override;
};
#endif // ICEEFFECT_HPP

