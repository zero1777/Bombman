#ifndef SPELLRANGE_HPP
#define SPELLRANGE_HPP
#include <string>
#include <vector>

#include "Sprite.hpp"


class PlayScene;

namespace Engine {
	struct Point;
}  // namespace Engine


class SpellRange : public Engine::Sprite {
protected:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
		TILE_WALL,
		TILE_BOX,
		TILE_BOMB
	};

	float damage;
	int range = 2;
	float timeTicks;
	float timeSpan = 3;
	bool isP1;
	PlayScene* getPlayScene();
	std::vector<Engine::Point> directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
public:
	explicit SpellRange(std::string img, float x, float y, float w, float h);
	void Update(float deltaTime) override;
};
#endif // SPELLRANGE_HPP

