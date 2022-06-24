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
#include "Ice.hpp"
#include <iostream>


PlayScene* Ice::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Ice::Ice(std::string img, float x, float y, float w, float h) :
	Engine::Sprite(img, x, y, w, h) {
	Velocity.x = Velocity.y = 0;
}

void Ice::Update(float deltaTime) {
	int x = (int)Position.x / 64;
	int y = (int)Position.y / 64;
	int x1 = (int)getPlayScene()->P1->Position.x / 64;
	int y1 = (int)getPlayScene()->P1->Position.y / 64;
	int x2 = (int)getPlayScene()->P2->Position.x / 64;
	int y2 = (int)getPlayScene()->P2->Position.y / 64;
	if (x == x1 && y == y1) {
		getPlayScene()->AddIceIcon(1);
		getPlayScene()->P1_IceValid = true;
		AudioHelper::PlayAudio("get_item.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}

	else if (x == x2 && y == y2) {
		getPlayScene()->AddIceIcon(2);
		getPlayScene()->P2_IceValid = true;
		AudioHelper::PlayAudio("get_item.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	Sprite::Update(deltaTime);
}