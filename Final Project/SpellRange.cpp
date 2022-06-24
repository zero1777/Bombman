#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>

#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "BombEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "SpellRange.hpp"


PlayScene* SpellRange::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

SpellRange::SpellRange(std::string img, float x, float y, float w, float h) :
	Engine::Sprite(img, x, y, w, h) {
	Velocity.x = Velocity.y = 0;
}

void SpellRange::Update(float deltaTime) {
	int x = (int)Position.x / 64;
	int y = (int)Position.y / 64;
	int x1 = (int)getPlayScene()->P1->Position.x / 64;
	int y1 = (int)getPlayScene()->P1->Position.y / 64;
	int x2 = (int)getPlayScene()->P2->Position.x / 64;
	int y2 = (int)getPlayScene()->P2->Position.y / 64;
	if (x == x1 && y == y1) {
		getPlayScene()->P1->bombrange++;
		getPlayScene()->P1RangeUpdate();
		AudioHelper::PlayAudio("get_item.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	else if (x == x2 && y == y2) {
		getPlayScene()->P2->bombrange++;
		getPlayScene()->P2RangeUpdate();
		AudioHelper::PlayAudio("get_item.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	Sprite::Update(deltaTime);
}