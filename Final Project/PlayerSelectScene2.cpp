#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "PlayerSelectScene2.hpp"
#include "Point.hpp"
#include "Image.hpp"
#include "GamePlayScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"

void PlayerSelectScene2::Initialize() {
	// TODO 1 (2/2): You can imitate the 2 files: 'LoseScene.hpp', 'LoseScene.cpp' to implement your start scene.
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("PlayerSelect/PlayerSelectBackground.png", 0, 0, w, h));
	AddNewObject(new Engine::Image("PlayerSelect/P2_image.png", 150, halfH * 2 / 3, 310, 400));
	AddNewObject(new Engine::Image("PlayerSelect/P1_image.png", 800, halfH * 2 / 3, 310, 400));
	AddNewObject(new Engine::Label("P1", "pirulen.ttf", 60, 300, halfH * 2 / 3 - 100, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label("P2", "pirulen.ttf", 60, 950, halfH * 2 / 3 - 100, 255, 255, 255, 255, 0.5, 0.5));

	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW + 400, halfH * 3 / 2 + 50, 300, 100);
	btn->SetOnClickCallback(std::bind(&PlayerSelectScene2::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW + 550, halfH * 3 / 2 + 100, 0, 0, 0, 255, 0.5, 0.5));
	//AudioHelper::PlayAudio("lose.wav");
}
void PlayerSelectScene2::BackOnClick(int stage) {
	// Change to select scene.
	/*GamePlayScene* scene = dynamic_cast<GamePlayScene*>(Engine::GameEngine::GetInstance().GetScene("GamePlayScene"));
	scene->MapId = stage;
	Engine::GameEngine::GetInstance().ChangeScene("GamePlayScene");*/
	//Engine::GameEngine::GetInstance().ChangeScene("stage-select");
	PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
	scene->MapId = stage;
	Engine::GameEngine::GetInstance().ChangeScene("play");
}

void PlayerSelectScene2::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode == ALLEGRO_KEY_TAB) {
		Engine::GameEngine::GetInstance().ChangeScene("PlayerSelectScene");
	}
}