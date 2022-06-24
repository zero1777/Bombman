#ifndef ROCKETBULLET_HPP
#define ROCKETBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
	struct Point;
}  // namespace Engine

class rocket_bullet : public Bullet {
public:
	explicit rocket_bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
	void OnExplode(Enemy* enemy) override;
};
#endif // ROCKETBULLET_HPP
