#include <cmath>
#include <string>

#include "IceEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include <iostream>


PlayScene* IceEffect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
IceEffect::IceEffect(std::string img, float x, float y, float w, float h, int id) : Sprite(img , x, y, w, h), timeTicks(0), damage(damage), ID(id) {
	Velocity.x = Velocity.y = 0;
}
void IceEffect::Update(float deltaTime) {
	timeTicks += deltaTime;
	Sprite::Update(deltaTime);
	int x = (int)Position.x / 64;
	int y = (int)Position.y / 64;
	int x1 = (int)getPlayScene()->P1->Position.x / 64;
	int y1 = (int)getPlayScene()->P1->Position.y / 64;
	int x2 = (int)getPlayScene()->P2->Position.x / 64;
	int y2 = (int)getPlayScene()->P2->Position.y / 64;
	std::cout << x << ' ' << y << ' ' << x1 << ' ' << y1 << '\n';
	std::cout << damage << '\n';
	
	/*if (!P1damaged && x == x1 && y == y1) {
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
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		getPlayScene()->isBoxEliminate[y][x] = true;
		return;
	}*/
	if (ID == 2) {
		if (x == x1 && y == y1 && !P1_getFreezed) {
			timeSpan += 2; 
			P1_getFreezed = true;
			getPlayScene()->P1_GetFreezed = true;
		}
	}
	if (ID == 1) {
		if (x == x2 && y == y2 && !P2_getFreezed) {
			timeSpan += 2;
			P2_getFreezed = true;
			getPlayScene()->P2_GetFreezed = true;
		}
	}
	if (timeTicks >= timeSpan) {
		if (ID == 2) {
			if (timeSpan == 2.5) {
				getPlayScene()->P1_GetFreezed = false;
			}
		}
		if (ID == 1) {
			if (timeSpan == 2.5) {
				getPlayScene()->P2_GetFreezed = false;
			}
		}
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	
}
