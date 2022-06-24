#ifndef SHOES_HPP
#define SHOES_HPP
#include <string>
#include <vector>

#include "Sprite.hpp"


class PlayScene;

namespace Engine {
	struct Point;
}  // namespace Engine


class Shoes : public Engine::Sprite {
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
	float timeSpan = 5;
	int PlayerID;
	PlayScene* getPlayScene();
	std::vector<Engine::Point> directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
public:
	explicit Shoes(std::string img, float x, float y, float w, float h);
	void Update(float deltaTime) override;
};
#endif // SHOES_HPP

