#ifndef ROCKETTURRET_HPP
#define ROCKETTURRET_HPP
#include "Turret.hpp"

class rocket_turret : public Turret {
public:
	static const int Price;
	rocket_turret(float x, float y);
	void CreateBullet() override;
};
#endif // ROCKETTURRET_HPP
