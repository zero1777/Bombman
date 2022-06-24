// [main.cpp]
// this template is provided for the 2D tower defense game.
// Program entry point.
// Returns program exit code.
#include "GameEngine.hpp"
#include "LOG.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "WinScene.hpp"
#include "StartScene.hpp"
#include "PlayerSelectScene.hpp"
#include "PlayerSelectScene2.hpp"
#include "P1WinScene.hpp"
#include "P2WinScene.hpp"
#include "GamePlayScene.hpp"


int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("PlayerSelectScene", new PlayerSelectScene());
	game.AddNewScene("PlayerSelectScene2", new PlayerSelectScene2());
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("StartScene", new StartScene());
	game.AddNewScene("p1winscene", new P1WinScene());
	game.AddNewScene("p2winscene", new P2WinScene());
	game.AddNewScene("GamePlayScene", new GamePlayScene());
	// TODO 1 (1/2): Add a New Scene here and change the start scene below.
	game.Start("StartScene", 60, 1600, 832);
	return 0;
}
