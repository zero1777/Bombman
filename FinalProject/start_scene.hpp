#ifndef STARTSCENE_HPP
#define STARTSCENE_HPP
#include "IScene.hpp"

class start_scene final : public Engine::IScene {
public:
	explicit start_scene() = default;
	void Initialize() override;
	void BackOnClick(int stage);
};

#endif // STARTSCENE_HPP
