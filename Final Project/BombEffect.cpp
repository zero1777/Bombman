#include <cmath>
#include <string>

#include "BombEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include <iostream>


PlayScene* BombEffect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
BombEffect::BombEffect(float x, float y, int damage) : Sprite("play/bomb_effect.jpg", x, y, 64, 64 ), timeTicks(0), damage(damage){
	Velocity.x = Velocity.y = 0;
	P1damaged = false;
	P2damaged = false;
}
void BombEffect::Update(float deltaTime) {
	timeTicks += deltaTime;
	int x = (int)Position.x / 64;
	int y = (int)Position.y / 64;
	int x1 = (int)getPlayScene()->P1->Position.x / 64;
	int y1 = (int)getPlayScene()->P1->Position.y / 64;
	int x2 = (int)getPlayScene()->P2->Position.x / 64;
	int y2 = (int)getPlayScene()->P2->Position.y / 64;
	/*std::cout << x << ' ' << y << ' ' << x1 << ' ' << y1 << '\n';
	std::cout << damage << '\n';*/
	if (!P1damaged && x == x1 && y == y1) {
		getPlayScene()->P1->Hit(damage);
		getPlayScene()->P1HPUpdate();
		P1damaged = true;
	}
	if (!P2damaged && x == x2 && y == y2) {
		getPlayScene()->P2->Hit(damage);
		getPlayScene()->P2HPUpdate();
		P2damaged = true;
	}
	if (timeTicks >= timeSpan) {
		getPlayScene()->isBoxEliminate[y][x] = true;
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	Sprite::Update(deltaTime);
}
