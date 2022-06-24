#ifndef BOMB_HPP
#define BOMB_HPP
#include <string>
#include <vector>

#include "Sprite.hpp"


class PlayScene;

namespace Engine {
	struct Point;
}  // namespace Engine


class Bomb : public Engine::Sprite {
protected:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
		TILE_WALL,
		TILE_BOX,
		TILE_BOMB
	};

	int damage;
	int range;
	float timeTicks;
	float timeSpan = 3;
	int PlayerID;
	PlayScene* getPlayScene();
	std::vector<Engine::Point> directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
public:
	int id;
	explicit Bomb(std::string img, int damage, float x, float y, int isP1, int bombrange, int id);
	void Update(float deltaTime) override;
};
#endif // BOMB_HPP
