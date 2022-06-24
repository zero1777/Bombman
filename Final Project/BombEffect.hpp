#ifndef BOMBEFFECT_HPP
#define BOMBEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

class BombEffect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	int damage;
	float timeSpan = 0.5;
	bool P1damaged;
	bool P2damaged;
public:
	BombEffect(float x, float y, int damage);
	void Update(float deltaTime) override;
};
#endif // BOMBEFFECT_HPP
