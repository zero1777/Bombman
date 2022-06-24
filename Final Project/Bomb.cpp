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
#include "Bomb.hpp"
#include <iostream>


PlayScene* Bomb::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Bomb::Bomb(std::string img, int damage, float x, float y, int id, int bombrange, int bombid) :
	Engine::Sprite(img, x, y, 64, 64), damage(damage), timeTicks(0), range(bombrange), id(bombid) {
	Velocity.x = Velocity.y = 0;
	this->PlayerID = id;
	//std::cout << "isP1: " << isP1 << std::endl;
}

void Bomb::Update(float deltaTime) {
	timeTicks += deltaTime;
	Sprite::Update(deltaTime);
	int x1 = (int)getPlayScene()->P1->Position.x / 64;
	int y1 = (int)getPlayScene()->P1->Position.y / 64;
	int x2 = (int)getPlayScene()->P2->Position.x / 64;
	int y2 = (int)getPlayScene()->P2->Position.y / 64;
	if (Velocity.x < 0) {
		int x = (int)Position.x / 64;
		int y = (int)Position.y / 64;
		if (getPlayScene()->mapState[y][x - 1] != TILE_FLOOR && Position.x <= x * 64 + 32
			|| (fabs(getPlayScene()->P1->Position.x - Position.x) <= 48 && fabs(getPlayScene()->P1->Position.y - Position.y) <= 16) 
			|| (fabs(getPlayScene()->P2->Position.x - Position.x) <= 48 && fabs(getPlayScene()->P2->Position.y - Position.y) <= 16)){
			Velocity.x = 0;
			Position.x = x * 64 + 32;
		}
		/*if (x - 1 == x1 && y == y1) getPlayScene()->P1->Position.x = Position.x - 128;
		if (x - 1 == x2 && y == y2) getPlayScene()->P2->Position.x = Position.x - 128;*/
		getPlayScene()->isbombhere[y][x] = true;
		getPlayScene()->isBoxEliminate[y][x + 1] = true;
	}
	else if (Velocity.x > 0) {
		int x = (int)Position.x / 64;
		int y = (int)Position.y / 64;
		if (getPlayScene()->mapState[y][x + 1] != TILE_FLOOR && Position.x >= x * 64 + 32
			|| (fabs(getPlayScene()->P1->Position.x - Position.x) <= 48 && fabs(getPlayScene()->P1->Position.y - Position.y) <= 16)
			|| (fabs(getPlayScene()->P2->Position.x - Position.x) <= 48 && fabs(getPlayScene()->P2->Position.y - Position.y) <= 16)) {
			Velocity.x = 0;
			Position.x = x * 64 + 32;
		}
		/*if (x + 1 == x1 && y == y1) getPlayScene()->P1->Position.x = Position.x + 128;
		if (x + 1 == x2 && y == y2) getPlayScene()->P2->Position.x = Position.x + 128;*/
		getPlayScene()->isbombhere[y][x] = true;
		getPlayScene()->isBoxEliminate[y][x - 1] = true;
	}
	else if (Velocity.y < 0) {
		int x = (int)Position.x / 64;
		int y = (int)Position.y / 64;
		if (getPlayScene()->mapState[y - 1][x] != TILE_FLOOR && Position.y <= y * 64 + 32
			|| (fabs(getPlayScene()->P1->Position.x - Position.x) <= 16 && fabs(getPlayScene()->P1->Position.y - Position.y) <= 48)
			|| (fabs(getPlayScene()->P2->Position.x - Position.x) <= 16 && fabs(getPlayScene()->P2->Position.y - Position.y) <= 48)) {
			Velocity.y = 0;
			Position.y = y * 64 + 32;
		}
		/*if (x == x1 && y - 1 == y1) getPlayScene()->P1->Position.y = Position.y - 128;
		if (x == x2 && y - 1 == y2) getPlayScene()->P2->Position.y = Position.y - 128;*/
		getPlayScene()->isbombhere[y][x] = true;
		getPlayScene()->isBoxEliminate[y + 1][x] = true;
	}
	else if (Velocity.y > 0) {
		int x = (int)Position.x / 64;
		int y = (int)Position.y / 64;
		if (getPlayScene()->mapState[y][x - 1] != TILE_FLOOR && Position.x >= y * 64 + 32
			|| (fabs(getPlayScene()->P1->Position.x - Position.x) <= 16 && fabs(getPlayScene()->P1->Position.y - Position.y) <= 48)
			|| (fabs(getPlayScene()->P2->Position.x - Position.x) <= 16 && fabs(getPlayScene()->P2->Position.y - Position.y) <= 48)) {
			Velocity.y = 0;
			Position.y = y * 64 + 32;
		}
		/*if (x == x1 && y + 1 == y1) getPlayScene()->P1->Position.y = Position.y + 128;
		if (x == x2 && y + 1 == y2) getPlayScene()->P2->Position.y = Position.y + 128;*/
		getPlayScene()->isbombhere[y][x] = true;
		getPlayScene()->isBoxEliminate[y - 1][x] = true;
	}

	if (timeTicks >= timeSpan && Velocity.x == 0 && Velocity.y == 0) {
		getPlayScene()->EffectGroup->AddNewObject(new BombEffect(Position.x, Position.y, damage));
		for (auto dir : directions) {
			for (int i = 1; i <= range; i++) {
				int x = (Position.x + dir.x * 64 * i - 32) / 64;
				int y = (Position.y + dir.y * 64 * i - 32) / 64;
				if (x < 0) x = 0;
				if (y < 0) y = 0;
				if (x > 19) x = 19;
				if (y > 12) y = 12;
				if (getPlayScene()->mapState[y][x] == TILE_WALL) break;
				if (getPlayScene()->mapState[y + dir.y][x + dir.x])
				/*else if (getPlayScene()->mapState[y][x] == TILE_BOX)
					getPlayScene()->isBoxEliminate[y][x] = true;*/

				getPlayScene()->EffectGroup->AddNewObject(new BombEffect(Position.x + dir.x * 64 * i, Position.y + dir.y * 64 * i, damage));
				if (getPlayScene()->mapState[y][x] == TILE_BOX) break;
			}
		}
		
		//std::cout << "isP1: " << isP1 << std::endl;
		if (PlayerID == 1) {
			std::cout << "Hello isP1\n";
			getPlayScene()->P1bombamount--;
		}
			
		else if (PlayerID == 2){
			std::cout << "Hello World\n";
			getPlayScene()->P2bombamount--;
		}
		getPlayScene()->isbombvalid[id] = true;
		AudioHelper::PlayAudio("bomb.wav");
		getPlayScene()->ItemGroup->RemoveObject(objectIterator);
	}
	
}