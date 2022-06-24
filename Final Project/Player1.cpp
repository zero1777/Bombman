#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>

#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "ExplosionEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "P2WinScene.hpp"
#include "Player1.hpp"
#include "Sprite.hpp"
#include "Label.hpp"
#include "Bomb.hpp"
#include "iostream"


const std::vector<int> Player1::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
									ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
									ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER, ALLEGRO_KEY_SPACE, 
									ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_W, ALLEGRO_KEY_A
									};

const int Player1::BlockSize = 64;

PlayScene* Player1::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}


Player1::Player1(std::string img, float hp, float damage, float x, float y, int screen_w, int screen_h) :
	Engine::Sprite(img, x, y, 64, 64), hp(hp), damage(damage) , screen_w(screen_w), screen_h(screen_h), bombrange(2) {
	Velocity.x = 0;
	Velocity.y = 0;
	timeTicks = 0;
}

float Player1::GetDamage() {
	return damage;
}

float Player1::Gethp() {
	return hp;
}

int Player1::GetRange() {
	return bombrange;
}

void Player1::Hit(float damage) {
	hp -= damage;
	if (hp <= 0) {
		PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
		P2WinScene* P2scene = dynamic_cast<P2WinScene*>(Engine::GameEngine::GetInstance().GetScene("p2winscene"));
		P2scene->MapId = scene->MapId;
		Engine::GameEngine::GetInstance().ChangeScene("p2winscene");
	}
		
}

void Player1::Enhance(float value) {
	damage += value;
}

void Player1::Update(float deltaTime) {
	Sprite::Update(deltaTime);

	// bound the player
	if (Position.x - GetBitmapWidth() / 2 <= 0)
		Position.x = GetBitmapWidth() / 2;
	else if (Position.x + GetBitmapWidth() / 2 >= screen_w)
		Position.x = screen_w - GetBitmapWidth()/2;

	if (Position.y - GetBitmapHeight() / 2 <= 0)
		Position.y = GetBitmapHeight() / 2;
	else if (Position.y + GetBitmapHeight() / 2 >= screen_h)
		Position.y = screen_h - GetBitmapHeight()/2;

	int x = (int)(Position.x) / 64;
	int y = (int)(Position.y) / 64;
	if (x < 1) x = 1;
	if (x > 18) x = 18;
	if (y < 1) y = 1;
	if (y > 11) y = 11;

	for (int i = (int)Position.x - 16; i <= (int)Position.x + 16; i += 16) {
		int px = i / 64;
		if (getPlayScene()->mapState[y - 1][px] != TILE_FLOOR && Position.y <= y * 64 + 32) {
			Velocity.y = 0;
			Position.y = y * 64 + 32;
			bounded_up = true;
			//std::cout << 1 << ' ';
		}
		else {
			bounded_up = false;
			//std::cout << 0 << ' ';
		}
	}
	for (int i = (int)Position.x - 16; i <= (int)Position.x + 16; i += 16) {
		int px = i / 64;
		if (getPlayScene()->mapState[y + 1][px] != TILE_FLOOR && Position.y >= y * 64 + 32) {
			Velocity.y = 0;
			Position.y = y * 64 + 32;
			bounded_down = true;
			//std::cout << 1 << ' ';
		}
		else {
			bounded_down = false;
			//std::cout << 0 << ' ';
		}
	}
	for (int i = (int)Position.y - 16; i <= (int)Position.y + 16; i += 16) {
		int py = i / 64;
		if (getPlayScene()->mapState[py][x - 1] != TILE_FLOOR && Position.x <= x * 64 + 32) {
			Velocity.x = 0;
			Position.x = x * 64 + 32;
			bounded_left = true;
			//std::cout << 1 << ' ';
		}
		else {
			bounded_left = false;
			//std::cout << 0 << ' ';
		}
	}
	for (int i = (int)Position.y - 16; i <= (int)Position.y + 16; i += 16) {
		int py = i / 64;
		if (getPlayScene()->mapState[py][x + 1] != TILE_FLOOR && Position.x >= x * 64 + 32) {
			Velocity.x = 0;
			Position.x = x * 64 + 32;
			bounded_right = true;
			//std::cout << 1 << '\n';
		}
		else {
			bounded_right = false;
			//std::cout << 0 << '\n';
		}
	}
	if (kickmode) {
		timeTicks += deltaTime;
		getPlayScene()->UIP1CD->Text = std::string("Shoes CD: ") + std::to_string(timeSpan-timeTicks);
	}
		
	if (kickmode && timeTicks > timeSpan) {
		timeTicks = 0;
		getPlayScene()->UIP1CD->Text = std::string("Shoes CD: ");
		kickmode = false;
	}
	/*if (bounded_up)
		std::cout << 1 << ' ';
	else
		std::cout << 0 << ' ';
	if (bounded_down)
		std::cout << 1 << ' ';
	else
		std::cout << 0 << ' ';
	if (bounded_left)
		std::cout << 1 << ' ';
	else
		std::cout << 0 << ' ';
	if (bounded_right)
		std::cout << 1 << '\n';
	else
		std::cout << 0 << '\n';*/

}

void Player1::OnKeyDown(int keycode) {
	/*if (keycode == ALLEGRO_KEY_UP)
		Position.y -= 64;
	else if (keycode == ALLEGRO_KEY_DOWN)
		Position.y += 64;
	else if (keycode == ALLEGRO_KEY_LEFT)
		Position.x -= 64;
	else if (keycode == ALLEGRO_KEY_RIGHT)
		Position.x += 64;*/
	if (keycode == ALLEGRO_KEY_W) {
		Velocity.y = -240;
		if (kickmode) {
			int i;
			int x = (int)(Position.x) / 64;
			int y = (int)(Position.y) / 64;
			if (y <= 1) y = 1;
			if (getPlayScene()->mapState[y - 1][x] == TILE_BOMB) {
				for (i = 0; i < 6; i++) {
					if (!getPlayScene()->isbombvalid[i]) {
						int bx = (int)getPlayScene()->bomb[i]->Position.x / 64;
						int by = (int)getPlayScene()->bomb[i]->Position.y / 64;
						if (x == bx && y - 1 == by) {
							AudioHelper::PlayAudio("kick.wav");
							getPlayScene()->bomb[i]->Velocity.y = -300;
							break;
						}
					}
				}
			}
		}
	}
	else if (keycode == ALLEGRO_KEY_S) {
		Velocity.y = 240;
		if (kickmode) {
			int i;
			int x = (int)(Position.x) / 64;
			int y = (int)(Position.y) / 64;
			if (y >= 18) y = 18;
			if (getPlayScene()->mapState[y + 1][x] == TILE_BOMB) {
				for (i = 0; i < 6; i++) {
					if (!getPlayScene()->isbombvalid[i]) {
						int bx = (int)getPlayScene()->bomb[i]->Position.x / 64;
						int by = (int)getPlayScene()->bomb[i]->Position.y / 64;
						if (x == bx && y + 1 == by) {
							AudioHelper::PlayAudio("kick.wav");
							getPlayScene()->bomb[i]->Velocity.y = 300;
							break;
						}
					}
				}
			}
		}
	}
	else if (keycode == ALLEGRO_KEY_A) {
		Velocity.x = -240;
		if (kickmode) {
			int i;
			int x = (int)(Position.x) / 64;
			int y = (int)(Position.y) / 64;
			if (x <= 1) x = 1;
			if (getPlayScene()->mapState[y][x - 1] == TILE_BOMB) {
				for (i = 0; i < 6; i++) {
					if (!getPlayScene()->isbombvalid[i]) {
						int bx = (int)getPlayScene()->bomb[i]->Position.x / 64;
						int by = (int)getPlayScene()->bomb[i]->Position.y / 64;
						if (x - 1 == bx && y == by) {
							AudioHelper::PlayAudio("kick.wav");
							getPlayScene()->bomb[i]->Velocity.x = -300;
							break;
						}
					}
				}
			}
		}
	}
	else if (keycode == ALLEGRO_KEY_D) {
		Velocity.x = 240;
		int i;
		if (kickmode) {
			int x = (int)(Position.x) / 64;
			int y = (int)(Position.y) / 64;
			if (x >= 18) x = 18;
			if (getPlayScene()->mapState[y][x + 1] == TILE_BOMB) {
				for (i = 0; i < 6; i++) {
					if (!getPlayScene()->isbombvalid[i]) {
						int bx = (int)getPlayScene()->bomb[i]->Position.x / 64;
						int by = (int)getPlayScene()->bomb[i]->Position.y / 64;
						if (x + 1 == bx && y == by) {
							AudioHelper::PlayAudio("kick.wav");
							getPlayScene()->bomb[i]->Velocity.x = 300;
							break;
						}
					}
				}
			}
		}
	}
}

void Player1::OnKeyUp(int keycode) {
	int t;
	if (keycode == ALLEGRO_KEY_W && !bounded_up) {
		t = (int)Position.y;
		Velocity.y = 0;
		//Position.y = t / 64 * 64 + 32;
	}
	else if (keycode == ALLEGRO_KEY_S && !bounded_down) {
		t = (int)Position.y;
		Velocity.y = 0;
		//Position.y = t / 64 * 64 + 32;
	}
	else if (keycode == ALLEGRO_KEY_A && !bounded_left) {
		t = (int)Position.x;
		Velocity.x = 0;
		//Position.x = t / 64 * 64 + 32;
	}
	else if (keycode == ALLEGRO_KEY_D && !bounded_right) {
		t = (int)Position.x;
		Velocity.x = 0;
		//Position.x = t / 64 * 64 + 32;
	}
}