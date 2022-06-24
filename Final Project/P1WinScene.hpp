#ifndef P1WINSCENE_HPP
#define P1WINSCENE_HPP
#include "IScene.hpp"

class P1WinScene final : public Engine::IScene {
public:
	int MapId;
	explicit P1WinScene() = default;
	void Initialize() override;
	void BackOnClick(int stage);
};

#endif // STARTSCENE_HPP
