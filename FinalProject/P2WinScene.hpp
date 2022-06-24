#ifndef P2WINSCENE_HPP
#define P2WINSCENE_HPP
#include "IScene.hpp"

class P2WinScene final : public Engine::IScene {
public:
	int MapId;
	explicit P2WinScene() = default;
	void Initialize() override;
	void BackOnClick(int stage);
};

#endif // STARTSCENE_HPP
