#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <queue>
#include <string>
#include <memory>
#include <string>

#include "IScene.hpp"
#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "IceEffect.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "LaserTurret.hpp"
#include "MachineGunTurret.hpp"
#include "MissileTurret.hpp"
#include "PlugGunTurret.hpp"
#include "Plane.hpp"
#include "PlaneEnemy.hpp"
#include "PlayScene.hpp"
#include "SoldierEnemy.hpp"
#include "RedNormalEnemy.hpp"
#include "Sprite.hpp"
#include "TankEnemy.hpp"
#include "Turret.hpp"
#include "TurretButton.hpp"
#include "Player1.hpp"
#include "Player2.hpp"
#include "Bomb.hpp"
#include "Thunder.hpp"
#include "SpellRange.hpp"
#include "Ice.hpp"
#include "Shoes.hpp"
#include <iostream>

//PlaySceneP1("win/P1_win.png", 100, 10, 500, 500, 1280, 832);
bool PlayScene::DebugMode = false;
bool PlayScene::isKeyDown = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
									ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
									ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER, 
									ALLEGRO_KEY_L,ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_W, ALLEGRO_KEY_A
									};
Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
	// TODO 5 (1/2): There's a bug in this file, which crashes the game when you win. Try to find it.
	// TODO 5 (2/2): There's a cheat code in this file. Try to find it.
	mapState.clear();
	keyStrokes.clear();
	ticks = 0;
	lives = 10;
	money = 150;
	SpeedMult = 1;
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(ItemGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	if (MapId == 1) {
		AddNewObject(P1 = new Player1("win/P1_win.png", 100, 30, 160, 608, 1280, 832));
		AddNewObject(P2 = new Player2("win/P2_win.png", 100, 30, 1120, 224, 1280, 832));
	}
	else if (MapId == 2) {
		AddNewObject(P1 = new Player1("win/P2_win.png", 100, 30, 160, 608, 1280, 832));
		AddNewObject(P2 = new Player2("win/P1_win.png", 100, 30, 1120, 224, 1280, 832));
	}
	
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();
	ReadEnemyWave();
	mapDistance = CalculateBFSDistance();
	ConstructUI();
	imgTarget = new Engine::Image("play/target.png", 0, 0);
	imgTarget->Visible = false;
	preview = nullptr;
	UIGroup->AddNewObject(imgTarget);
	AudioHelper::PlayBGM("play.ogg");
	P1bombamount = 0;
	P2bombamount = 0;
	for (int i = 0; i < 6; i++) isbombvalid[i] = true;
}
void PlayScene::Update(float deltaTime) {
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	/*std::cout << "P1bombamount: " << P1bombamount << std::endl;
	std::cout << "P2bombamount: " << P2bombamount << std::endl;*/
	for (int i = 0; i < SpeedMult; i++) {
		IScene::Update(deltaTime);
		// Check if we should create new enemy.
		ticks += deltaTime;
		if (enemyWaveData.empty()) {
			if (EnemyGroup->GetObjects().empty()) {
				// Free resources.
				/*delete TileMapGroup;
				delete GroundEffectGroup;
				delete DebugIndicatorGroup;
				delete TowerGroup;
				delete EnemyGroup;
				delete BulletGroup;
				delete EffectGroup;
				delete UIGroup;
				delete imgTarget;*/
				// Win.
				//Engine::GameEngine::GetInstance().ChangeScene("win-scene");
				//Engine::GameEngine::GetInstance().ChangeScene("win");
			}
			continue;
		}
		auto current = enemyWaveData.front();
		if (ticks < current.second)
			continue;
		ticks -= current.second;
		enemyWaveData.pop_front();
		const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
		Enemy* enemy;
		switch (current.first) {
		case 1:
			EnemyGroup->AddNewObject(enemy = new SoldierEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
		case 2:
			EnemyGroup->AddNewObject(enemy = new PlaneEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
		case 3:
			EnemyGroup->AddNewObject(enemy = new TankEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
		case 4:
			EnemyGroup->AddNewObject(enemy = new RedNormalEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
	// TODO 2 (7/8): You need to modify 'resources/enemy1.txt', or 'resources/enemy2.txt' to spawn the 4th enemy.
	//         The format is "[EnemyId] [TimeDelay] [Repeat]".
	// TODO 2 (8/8): Enable the creation of the 4th enemy.
		default:
			continue;
		}
		enemy->UpdatePath(mapDistance);
		// Compensate the time lost.
		enemy->Update(ticks);
	}
	if (preview) {
		preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
		// To keep responding when paused.
		preview->Update(deltaTime);
	}
	/*int y1 = P1->Position.y / BlockSize;
	int x1 = P1->Position.x / BlockSize;
	int tmpy1 = P1->Position.y - P1->GetBitmapHeight() / 2;
	int tmpx1 = P1->Position.x - P1->GetBitmapWidth() / 2;
	std::cout << x1 << ' ' << y1 << '\n';
	if (isKeyDown) {
		if (mapState[y1 - 1][x1] != TILE_FLOOR && P1->Position.y < y1 * 64 + 32) {
			P1->Velocity.y = 0;
			P1->Position.y = y1 * 64 + 32;
		}
		else if (mapState[y1 + 1][x1] != TILE_FLOOR && P1->Position.y > y1 * 64 - 32) {
			P1->Velocity.y = 0;
			P1->Position.y = y1 * 64 - 32;
		}
		else if (mapState[y1][x1 - 1] != TILE_FLOOR && P1->Position.x < x1 * 64 + 32) {
			P1->Velocity.x = 0;
			P1->Position.x = x1 * 64 + 32;
		}
		else if (mapState[y1][x1 + 1] != TILE_FLOOR && P1->Position.x > x1 * 64 - 32) {
			P1->Velocity.x = 0;
			P1->Position.x = x1 * 64 - 32;
		}
	}*/
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			if (isbombhere[i][j]) {
				mapState[i][j] = TILE_BOMB;
				isbombhere[i][j] = false;
			}
		}
	}

	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			if (isBoxEliminate[i][j]) {
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.jpg", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				if (mapState[i][j] == TILE_BOX && (i+j) % 5 == 0)
					ItemGroup->AddNewObject(new Thunder("play/thunder.png", j * BlockSize+32, i * BlockSize+32, BlockSize, BlockSize));
				else if (mapState[i][j] == TILE_BOX && (i + j) % 6 == 0) 
					ItemGroup->AddNewObject(new SpellRange("play/spellrange.png", j * BlockSize + 32, i * BlockSize + 32, BlockSize, BlockSize));
				else if (mapState[i][j] == TILE_BOX && (i + j) % 4 == 0)
					ItemGroup->AddNewObject(new Ice("play/ice.png", j * BlockSize + 32, i * BlockSize + 32, BlockSize, BlockSize));
				else if (mapState[i][j] == TILE_BOX && ((i + j) % 7 == 0 || (i + j) % 3 == 0))
					ItemGroup->AddNewObject(new Shoes("play/shoes.png", j* BlockSize + 32, i * BlockSize + 32, BlockSize, BlockSize));
				mapState[i][j] = TILE_FLOOR;
				isBoxEliminate[i][j] = false;
			}
		}
	}
	
}
void PlayScene::Draw() const {
	IScene::Draw();
	/*if (DebugMode) {
		// Draw reverse BFS distance on all reachable blocks.
		for (int i = 0; i < MapHeight; i++) {
			for (int j = 0; j < MapWidth; j++) {
				if (mapDistance[i][j] != -1) {
					// Not elegant nor efficient, but it's quite enough for debugging.
					Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
					//Engine::Image label("GamePlayScene/wall.jpg", 0, 0, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
					label.Anchor = Engine::Point(0.5, 0.5);
					label.Draw();
				}
			}
		}
	}*/
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = x * BlockSize;
	imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (button & 1) {
		if (mapState[y][x] != TILE_OCCUPIED) {
			if (!preview)
				return;
			// Check if valid.
			if (!CheckSpaceValid(x, y)) {
				Engine::Sprite* sprite;
				GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
				sprite->Rotation = 0;
				return;
			}
			// Purchase.
			EarnMoney(-preview->GetPrice());
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
			preview->Position.x = x * BlockSize + BlockSize / 2;
			preview->Position.y = y * BlockSize + BlockSize / 2;
			preview->Enabled = true;
			preview->Preview = false;
			preview->Tint = al_map_rgba(255, 255, 255, 255);
			TowerGroup->AddNewObject(preview);
			// To keep responding when paused.
			preview->Update(0);
			// Remove Preview.
			preview = nullptr;

			mapState[y][x] = TILE_OCCUPIED;
			OnMouseMove(mx, my);
		}
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	isKeyDown = true;
	if (!P1_GetFreezed)
		P1->OnKeyDown(keyCode);
	if (!P2_GetFreezed)
		P2->OnKeyDown(keyCode);
	// P1 movement
	int y1 = P1->Position.y / BlockSize;
	int x1 = P1->Position.x / BlockSize;
	/*int tmpy1 = P1->Position.y - P1->GetBitmapHeight() / 2;
	int tmpx1 = P1->Position.x - P1->GetBitmapWidth() / 2;
	if (keyCode == ALLEGRO_KEY_UP) {
		if (tmpy1 / BlockSize > 0) {
			if (mapState[tmpy1 / BlockSize - 1][x1] == TILE_FLOOR) P1->OnKeyDown(keyCode);
		}
	}
	else if (keyCode == ALLEGRO_KEY_DOWN) {
		if (tmpy1 / BlockSize < MapHeight-1) {
			if (mapState[tmpy1 / BlockSize + 1][x1] == TILE_FLOOR) P1->OnKeyDown(keyCode);
		}
	}
	else if (keyCode == ALLEGRO_KEY_LEFT) {
		if (tmpx1 / BlockSize > 0) {
			if (mapState[y1][tmpx1 / BlockSize - 1] == TILE_FLOOR) P1->OnKeyDown(keyCode);
		}
	}
	else if (keyCode == ALLEGRO_KEY_RIGHT) {
		if (tmpx1 / BlockSize < MapWidth - 1) {
			if (mapState[y1][tmpx1 / BlockSize + 1] == TILE_FLOOR) P1->OnKeyDown(keyCode);
		}
	}
	*/
	// P2 movement
	int y2 = P2->Position.y / BlockSize;
	int x2 = P2->Position.x / BlockSize;
	/*int tmpy2 = P2->Position.y - P2->GetBitmapHeight() / 2;
	int tmpx2 = P2->Position.x - P2->GetBitmapWidth() / 2;
	if (keyCode == ALLEGRO_KEY_W) {
		if (tmpy2 / BlockSize > 0) {
			if (mapState[tmpy2 / BlockSize - 1][x2] == TILE_FLOOR) P2->OnKeyDown(keyCode);
		}
	}
	else if (keyCode == ALLEGRO_KEY_S) {
		if (tmpy2 / BlockSize < MapHeight - 1) {
			if (mapState[tmpy2 / BlockSize + 1][x2] == TILE_FLOOR) P2->OnKeyDown(keyCode);
		}
	}
	else if (keyCode == ALLEGRO_KEY_A) {
		if (tmpx2 / BlockSize > 0) {
			if (mapState[y2][tmpx2 / BlockSize - 1] == TILE_FLOOR) P2->OnKeyDown(keyCode);
		}
	}
	else if (keyCode == ALLEGRO_KEY_D) {
		if (tmpx2 / BlockSize < MapWidth - 1) {
			if (mapState[y2][tmpx2 / BlockSize + 1] == TILE_FLOOR) P2->OnKeyDown(keyCode);
		}
	}*/
	//std::cout << x1 << ' ' << y1 << '\n';
	/*if (keyCode == ALLEGRO_KEY_UP) {
		
		if (mapState[tmpy1/BlockSize-1][x1] == TILE_FLOOR) {
			if (tmp % BlockSize == 0)
				P1->Velocity.x = P1->Velocity.y = 0;
			else
				P1->OnKeyDown(keyCode);
			P1->OnKeyDown(keyCode);
		}
		else {
			P1->Velocity.x = P1->Velocity.y = 0;
		}
		//if (CheckSpaceValid(P1->Position.x/BlockSize, (P1->Position.y - 32) / BlockSize))  P1->OnKeyDown(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_LEFT) {
		if (mapState[y1][tmpx1 / BlockSize-1] == TILE_FLOOR)
			P1->OnKeyDown(keyCode);
		else {
			P1->Velocity.x = P1->Velocity.y = 0;
		}
	}
	else if (keyCode == ALLEGRO_KEY_DOWN) {
		if (mapState[tmpy1 / BlockSize][x1] == TILE_FLOOR)
			P1->OnKeyDown(keyCode);
		else {
			P1->Velocity.x = P1->Velocity.y = 0;
		}
	}
	else if (keyCode == ALLEGRO_KEY_RIGHT) {
		if (mapState[y1][tmpx1 / BlockSize] == TILE_FLOOR)
			P1->OnKeyDown(keyCode);
		else {
			P1->Velocity.x = P1->Velocity.y = 0;
		}
	}*/
	//else P1->OnKeyDown(keyCode);
	//if (y == 17)
	//P1->OnKeyDown(keyCode);
	/*else if (keyCode == ALLEGRO_KEY_DOWN && mapState[y][x] == TILE_FLOOR)
		P1->OnKeyDown(keyCode);
	else if (keyCode == ALLEGRO_KEY_RIGHT && mapState[y][x] == TILE_FLOOR)
		P1->OnKeyDown(keyCode);
	else if (keyCode == ALLEGRO_KEY_LEFT && mapState[y][x] == TILE_FLOOR)
		P1->OnKeyDown(keyCode);*/
	if (keyCode == ALLEGRO_KEY_SPACE && P1bombamount < 3 && mapState[y1][x1] == TILE_FLOOR && !P1_GetFreezed) {
		P1bombamount++;
		int x = (int)P1->Position.x / 64 * 64 + 32;
		int y = (int)P1->Position.y / 64 * 64 + 32;
		int i;
		for (i = 0; i < 6; i++) {
			if (isbombvalid[i]) {
				isbombvalid[i] = false;
				break;
			}
		};
		ItemGroup->AddNewObject(bomb[i] = new Bomb("play/bomb.png", P1->GetDamage(), x, y, 1, P1->bombrange, i));
		//ItemGroup->AddNewObject(new Bomb("play/bomb.png", 100, P1->Position.x, P1->Position.y, true, P1->bombrange));
		mapState[y1][x1] = TILE_BOMB;
		AudioHelper::PlayAudio("setBomb.wav");
	}
	if (keyCode == ALLEGRO_KEY_L && P2bombamount < 3 && mapState[y2][x2] == TILE_FLOOR && !P2_GetFreezed) {
		P2bombamount++; std::cout << "P2bombamount: " << P2bombamount << std::endl;
		int x = (int)P2->Position.x / 64 * 64 + 32;
		int y = (int)P2->Position.y / 64 * 64 + 32;
		int i;
		for (i = 0; i < 6; i++) {
			if (isbombvalid[i]) {
				isbombvalid[i] = false;
				break;
			}
		};

		ItemGroup->AddNewObject(bomb[i] = new Bomb("play/bomb.png", P2->GetDamage(), x, y, 2, P2->bombrange, i));
		//ItemGroup->AddNewObject(new Bomb("play/bomb.png", 100, P2->Position.x, P2->Position.y, false, P2->bombrange));
		mapState[y2][x2] = TILE_BOMB;
		AudioHelper::PlayAudio("setBomb.wav");
	}
	if (keyCode == ALLEGRO_KEY_I && P2_IceValid && mapState[y2][x2] == TILE_FLOOR && !P2_GetFreezed) {
		RemoveObject(P2_Iceimg->GetObjectIterator());
		P2_IceValid = false;

		for (auto dir : directions) {
			for (int i = 1; i <= 2; i++) {
				if (mapState[y2 + dir.y * i][x2 + dir.x * i] != TILE_FLOOR) break;
				ItemGroup->AddNewObject(new IceEffect("play/ice.png", x2 * 64 + 32 + dir.x * 64 * i, y2 * 64 + 32 + dir.y * 64 * i, 64, 64, 2));
			}
		}

		/*if (mapState[y2 + 1][x2] == TILE_FLOOR) 
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", x2 * BlockSize + 32, (y2 + 1)* BlockSize + 32, BlockSize, BlockSize, 2));
		if (mapState[y2 - 1][x2] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", x2* BlockSize + 32, (y2 - 1) * BlockSize + 32, BlockSize, BlockSize, 2));
		if (mapState[y2][x2 + 1] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", (x2 + 1) * BlockSize + 32, y2 * BlockSize + 32, BlockSize, BlockSize, 2));
		if (mapState[y2][x2 - 1] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", (x2 - 1) * BlockSize + 32, y2 * BlockSize + 32, BlockSize, BlockSize, 2));*/
		AudioHelper::PlayAudio("freeze.wav");
	}
	if (keyCode == ALLEGRO_KEY_R && P1_IceValid && mapState[y1][x1] == TILE_FLOOR && !P1_GetFreezed) {
		RemoveObject(P1_Iceimg->GetObjectIterator());
		P1_IceValid = false;

		for (auto dir : directions) {
			for (int i = 1; i <= 2; i++) {
				if (mapState[y1 + dir.y * i][x1 + dir.x * i] != TILE_FLOOR) break;
				ItemGroup->AddNewObject(new IceEffect("play/ice.png", x1 * 64 + 32 + dir.x * 64 * i, y1 * 64 + 32 + dir.y * 64 * i, 64, 64, 1));
			}
		}

		/*if (mapState[y1 + 1][x1] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", x1* BlockSize + 32, (y1 + 1) * BlockSize + 32, BlockSize, BlockSize, 1));
		if (mapState[y1 - 1][x1] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", x1* BlockSize + 32, (y1 - 1) * BlockSize + 32, BlockSize, BlockSize, 1));
		if (mapState[y1][x1 + 1] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", (x1 + 1)* BlockSize + 32, y1 * BlockSize + 32, BlockSize, BlockSize, 1));
		if (mapState[y1][x1 - 1] == TILE_FLOOR)
			ItemGroup->AddNewObject(new IceEffect("play/ice.png", (x1 - 1)* BlockSize + 32, y1 * BlockSize + 32, BlockSize, BlockSize, 1));*/
		AudioHelper::PlayAudio("freeze.wav");
	}
	if (keyCode == ALLEGRO_KEY_E && P1_ShoesValid && !P1_GetFreezed) {
		RemoveObject(P1_Shoesimg->GetObjectIterator());
		P1_ShoesValid = false;
		AudioHelper::PlayAudio("kickmode.wav");
		P1->kickmode = true;
	}
	if (keyCode == ALLEGRO_KEY_K && P2_ShoesValid && !P2_GetFreezed) {
		RemoveObject(P2_Shoesimg->GetObjectIterator());
		P2_ShoesValid = false;
		AudioHelper::PlayAudio("kickmode.wav");
		P2->kickmode = true;
	}
	/*if (keyCode == ALLEGRO_KEY_TAB) {
		DebugMode = !DebugMode;
	}
	else {
		keyStrokes.push_back(keyCode);
		if (keyStrokes.size() > code.size())
			keyStrokes.pop_front();
		if (keyCode == ALLEGRO_KEY_ENTER && keyStrokes.size() == code.size()) {
			auto it = keyStrokes.begin();
			for (int c : code) {
				if (*it != c && (c == ALLEGRO_KEYMOD_SHIFT && *it != ALLEGRO_KEY_LSHIFT && *it != ALLEGRO_KEY_RSHIFT))
					return;
				++it;
			}
			EffectGroup->AddNewObject(new Plane());
			money += 10000;
		}
	}*/
	/*if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for MachineGunTurret.
		UIBtnClicked(0);
	}
	else if (keyCode == ALLEGRO_KEY_W) {
		// Hotkey for LaserTurret.
		UIBtnClicked(1);
	}
	else if (keyCode == ALLEGRO_KEY_E) {
		// Hotkey for MissileTurret.
		UIBtnClicked(2);
	}
	// TODO 2 (5/8): Make the R key to create the 4th turret.
	else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		SpeedMult = keyCode - ALLEGRO_KEY_0;
	}
	else if (keyCode == ALLEGRO_KEY_R) {
		// Hotkey for PlugGunTurret
		UIBtnClicked(3);
	}*/
	Engine::Sprite sprite();
}
void PlayScene::OnKeyUp(int keyCode) {
	P1->OnKeyUp(keyCode);
	P2->OnKeyUp(keyCode);
	//isKeyDown = false;
	//ItemGroup->AddNewObject(new Bomb("play/bullet-5.png", 20, P1->Position.x, P1->Position.y));
}
void PlayScene::Hit() {
	lives--;
	UILives->Text = std::string("Life ") + std::to_string(lives);
	if (lives <= 0) {
		Engine::GameEngine::GetInstance().ChangeScene("lose");
	}
}
void PlayScene::P1HPUpdate() {
	UIP1HP->Text = std::string("HP: ") + std::to_string((int)P1->Gethp());
}
void PlayScene::P1RangeUpdate() {
	UIP1Range->Text = std::string("Range: ") + std::to_string((int)P1->GetRange());
}
void PlayScene::P1ATKUpdate() {
	UIP1ATK->Text = std::string("ATK: ") + std::to_string((int)P1->GetDamage());
}
void PlayScene::P2HPUpdate() {
	UIP2HP->Text = std::string("HP: ") + std::to_string((int)P2->Gethp());
}
void PlayScene::P2RangeUpdate() {
	UIP2Range->Text = std::string("Range: ") + std::to_string((int)P2->GetRange());
}
void PlayScene::P2ATKUpdate() {
	UIP2ATK->Text = std::string("ATK: ") + std::to_string((int)P2->GetDamage());
}
int PlayScene::GetMoney() const {
	return money;
}
void PlayScene::AddIceIcon(int ID) {
	if (ID == 1) 
		AddNewObject(P1_Iceimg = new Engine::Image("play/Ice.png", 1294, 178, 54, 54));
	else if (ID == 2) 
		AddNewObject(P2_Iceimg = new Engine::Image("play/Ice.png", 1294, 588, 54, 54));
}
void PlayScene::AddShoesIcon(int ID) {
	if (ID == 1)
		AddNewObject(P1_Shoesimg = new Engine::Image("play/shoes.png", 1344, 178, 54, 54));
	else if (ID == 2)
		AddNewObject(P2_Shoesimg = new Engine::Image("play/shoes.png", 1344, 588, 54, 54));
}
void PlayScene::EarnMoney(int money) {
	this->money += money;
	//UIMoney->Text = std::string("$") + std::to_string(this->money);
}
void PlayScene::ReadMap() {
	std::string filename = std::string("resources/map1") + ".txt";
	// Read map file.
	char c;
	std::vector<int> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '0': mapData.push_back(0); break;
		case '1': mapData.push_back(1); break;
		case '2': mapData.push_back(2); break;
		case '\n':
		case '\r': 
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.");
			break;
		default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();
	// Validate map data.
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.");
	// Store map in 2d array.
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			/*const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			if (num)
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			else
				TileMapGroup->AddNewObject(new Engine::Image("play/wall.jpg", j * BlockSize, i * BlockSize, BlockSize, BlockSize));*/
			TileMapGroup->AddNewObject(new Engine::Image("play/floor.jpg", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			const int num = mapData[i * MapWidth + j];
			if (num == 0) {
				TileMapGroup->AddNewObject(new Engine::Image("play/wall.jpg", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				mapState[i][j] = TILE_WALL;
			}
				
			else if (num == 1) {
				TileMapGroup->AddNewObject(new Engine::Image("play/box.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				mapState[i][j] = TILE_BOX;
			}

			else if (num == 2) {
				mapState[i][j] = TILE_FLOOR;
			}
			//if (i == 3 && j == 17) TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
		}
	}
}
void PlayScene::ReadEnemyWave() {
	/*std::string filename = std::string("resources/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	float type, wait, repeat;
	enemyWaveData.clear();
	std::ifstream fin(filename);
	while (fin >> type && fin >> wait && fin >> repeat) {
		for (int i = 0; i < repeat; i++)
			enemyWaveData.emplace_back(type, wait);
	}
	fin.close();*/
}
void PlayScene::ConstructUI() {
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
	if (MapId == 1) {
		AddNewObject(new Engine::Image("win/P1_win.png", 1500, 0, 54, 54));
		AddNewObject(new Engine::Image("win/P2_win.png", 1500, 410, 54, 54));
	}
	else if (MapId == 2) {
		AddNewObject(new Engine::Image("win/P2_win.png", 1500, 0, 54, 54));
		AddNewObject(new Engine::Image("win/P1_win.png", 1500, 410, 54, 54));
	}
	// Text
	// Player1 info
	UIGroup->AddNewObject(new Engine::Label(std::string("Player1"), "pirulen.ttf", 32, 1294, 0));
	UIGroup->AddNewObject(UIP1HP = new Engine::Label(std::string("HP: ") + std::to_string((int)P1->Gethp()), "pirulen.ttf", 24, 1294, 48));
	UIGroup->AddNewObject(UIP1ATK = new Engine::Label(std::string("ATK: ") + std::to_string((int)P1->GetDamage()), "pirulen.ttf", 24, 1294, 88));
	UIGroup->AddNewObject(UIP1Range = new Engine::Label(std::string("Range: ") + std::to_string((int)P1->GetRange()), "pirulen.ttf", 24, 1294, 128));
	UIGroup->AddNewObject(UIP1CD = new Engine::Label(std::string("Shoes CD: "), "pirulen.ttf", 24, 1294, 248));

	//Player2 info
	UIGroup->AddNewObject(new Engine::Label(std::string("Player2"), "pirulen.ttf", 32, 1294, 410));
	UIGroup->AddNewObject(UIP2HP = new Engine::Label(std::string("HP: ") + std::to_string((int)P2->Gethp()), "pirulen.ttf", 24, 1294, 458));
	UIGroup->AddNewObject(UIP2ATK = new Engine::Label(std::string("ATK: ") + std::to_string((int)P2->GetDamage()), "pirulen.ttf", 24, 1294, 498));
	UIGroup->AddNewObject(UIP2Range = new Engine::Label(std::string("Range: ") + std::to_string((int)P2->GetRange()), "pirulen.ttf", 24, 1294, 538));
	UIGroup->AddNewObject(UIP2CD = new Engine::Label(std::string("Shoes CD: "), "pirulen.ttf", 24, 1294, 658));
	
	/*TurretButton* btn;
	// Button 1
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1294, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-1.png", 1294, 136 - 8, 0, 0, 0, 0)
		, 1294, 136, MachineGunTurret::Price);
	// Reference: Class Member Function Pointer and std::bind.
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
	UIGroup->AddNewControlObject(btn);
	// Button 2
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1370, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-2.png", 1370, 136 - 8, 0, 0, 0, 0)
		, 1370, 136, LaserTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
	UIGroup->AddNewControlObject(btn);
	// Button 3
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1446, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-3.png", 1446, 136, 0, 0, 0, 0)
		, 1446, 136, MissileTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2));
	UIGroup->AddNewControlObject(btn);
	// TODO 2 (3/8): Create a button to support constructing the 4th tower.
	// Button 4
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/tower-base.png", 1522, 136, 0, 0, 0, 0),
		Engine::Sprite("play/turret-7.png", 1522, 136, 0, 0, 0, 0)
		, 1522, 136, PlugGunTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
	UIGroup->AddNewControlObject(btn);*/
}

void PlayScene::UIBtnClicked(int id) {
	if (preview)
		UIGroup->RemoveObject(preview->GetObjectIterator());
	if (id == 0 && money >= MachineGunTurret::Price)
		preview = new MachineGunTurret(0, 0);
	else if (id == 1 && money >= LaserTurret::Price)
		preview = new LaserTurret(0, 0);
	else if (id == 2 && money >= MissileTurret::Price)
		preview = new MissileTurret(0, 0);
	else if (id == 3 && money >= PlugGunTurret::Price)
		preview = new PlugGunTurret(0, 0);
	// TODO 2 (4/8): On callback, create the 4th tower.
	if (!preview)
		return;
	preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	preview->Tint = al_map_rgba(255, 255, 255, 200);
	preview->Enabled = false;
	preview->Preview = true;
	UIGroup->AddNewObject(preview);
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	if (map[0][0] == -1)
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0 || pnt.x >= MapWidth || pnt.y < 0 || pnt.y >= MapHeight)
			continue;
		if (map[pnt.y][pnt.x] == -1)
			return false;
	}
	// All enemy have path to exit.
	mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	std::queue<Engine::Point> que;
	// Push end point.
	// BFS from end point.
	if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
		return map;
	que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
	map[MapHeight - 1][MapWidth - 1] = 0;
	while (!que.empty()) {
		Engine::Point p = que.front();
		que.pop();
		// TODO 3 (1/1): Implement a BFS starting from the most right-bottom block in the map.
		//               For each step you should assign the corresponding distance to the most right-bottom block.
		//               mapState[y][x] is TILE_DIRT if it is empty.
		for (auto &c : directions) {
			int x = p.x + c.x;
			int y = p.y + c.y;
			if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight || map[y][x] != -1 || mapState[y][x] != TILE_DIRT) continue;
			else {
				map[y][x] = map[p.y][p.x] + 1;
				que.push(Engine::Point(x, y));
			}
		}
	}
	return map;
}
