#include <functional>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "P2WinScene.hpp"
#include "Point.hpp"
#include "Image.hpp"

void P2WinScene::Initialize() {

	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("win/win_background.png", 0, 0, w, h));
	AddNewObject(new Engine::Label("P2 WINS!!", "pirulen.ttf", 48, halfW, halfH / 2, 255, 255, 255, 255, 0.5, 0.5));
	if (MapId == 1) {
		AddNewObject(new Engine::Image("win/P1_lose.png", 150, halfH * 2 / 3, 310, 400));
		AddNewObject(new Engine::Image("win/P2_win.png", w - 310 - 150, halfH * 2 / 3, 310, 400));
	}
	else if (MapId == 2) {
		AddNewObject(new Engine::Image("win/P2_lose.png", 150, halfH * 2 / 3, 310, 400));
		AddNewObject(new Engine::Image("win/P1_win.png", w - 310 - 150, halfH * 2 / 3, 310, 400));
	}
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 + 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&P2WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("BACK", "pirulen.ttf", 48, halfW, halfH * 3 / 2 + 100, 0, 0, 0, 255, 0.5, 0.5));
	AudioHelper::PlayAudio("win.wav");
}
void P2WinScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("StartScene");
}
