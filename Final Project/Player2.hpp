#ifndef PLAYER2_HPP
#define PLAYER2_HPP
#include <string>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

namespace Engine {
	struct Point;
}  // namespace Engine


class Player2 : public Engine::Sprite {
protected:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
		TILE_WALL,
		TILE_BOX,
		TILE_BOMB
	};
	int screen_w, screen_h;
	float hp;
	float damage;
	float timeTicks;
	float timeSpan = 10;
	bool bounded_up, bounded_down, bounded_left, bounded_right;
	static const std::vector<int> code;
	PlayScene* getPlayScene();

public:
	Player2(std::string img, float hp, float damage, float x, float y, int screen_w, int screen_h);
	int bombrange;
	bool kickmode = false;
	void OnKeyDown(int keycode);
	void OnKeyUp(int keycode);
	void Hit(float damage);
	void Enhance(float value);
	void Update(float deltaTime) override;
	float GetDamage();
	float Gethp();
	int GetRange();
	static const int BlockSize;
	//std::vector<std::vector<TileType>> 

	
};

#endif // PLAYER2_HPP