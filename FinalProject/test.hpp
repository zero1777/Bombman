#ifndef TEST_HPP
#define TEST_HPP
#include "IScene.hpp"

class test final : public Engine::IScene {
public:
	explicit test() = default;
	void Initialize() override;
	void BackOnClick(int stage);
};

#endif // STARTSCENE_HPP


