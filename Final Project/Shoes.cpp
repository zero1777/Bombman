#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>

#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "IceEffect.hpp"
#include "GameEngine.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Shoes.hpp"
#include <iostream>


PlayScene* Shoes::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Shoes::Shoes(std::string img, float x, float y, float w, float h) :
	Engine::Sprite(img, x, y, w, h) {
	Velocity.x = Velocity.y = 0;
}

void Shoes::Update(float deltaTime) {
	timeTicks += deltaTime;
	int x = (int)Position.x / 64;
	int y = (int)Position.y / 64;
	int x1 = (int)getPlayScene()->P1->Position.x / 64;
	int y1 = (int)getPlayScene()->P1->Position.y / 64;
	int x2 = (int)getPlayScene()->P2->Position.x / 64;
	int y2 = (int)getPlayScene()->P2->Position.y / 64;
	if (x == x1 && y == y1) {
		getPlayScene()->AddShoesIcon(1);
		getPlayScene()->P1_ShoesValid = true;
		AudioHelper::PlayAudio("get_item.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	else if (x == x2 && y == y2) {
		getPlayScene()->AddShoesIcon(2);
		getPlayScene()->P2_ShoesValid = true;
		AudioHelper::PlayAudio("get_item.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	/*if (timeTicks >= timeSpan) {
		getPlayScene()->RemoveObject(objectIterator);
	}*/
	Sprite::Update(deltaTime);
}