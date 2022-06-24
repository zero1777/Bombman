#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <list>
#include <utility>
#include <vector>

#include "IScene.hpp"
#include "Point.hpp"
#include "Player1.hpp"
#include "Player2.hpp"
#include "Bomb.hpp"

class Turret;
class Bomb;
namespace Engine {
	class Group;
	class Image;
	class Label;
}  // namespace Engine

class PlayScene final : public Engine::IScene {
private:
	
protected:
	int lives;
	int money;
	int SpeedMult;
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
		TILE_WALL,
		TILE_BOX,
		TILE_BOMB
	};
public:
	static bool DebugMode;
	static bool isKeyDown;
	bool P1_bombValid;
	bool P2_bombValid;
	bool P1_IceValid;
	bool P2_IceValid;
	bool P1_ShoesValid;
	bool P2_ShoesValid;
	bool P1_GetFreezed;
	bool P2_GetFreezed;
	bool isBoxEliminate[13][20];
	bool isbombhere[13][20];
	bool isbombvalid[6];
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static const Engine::Point SpawnGridPoint;
	static const Engine::Point EndGridPoint;
	static const std::vector<int> code;
	int P1bombamount;
	int P2bombamount;
	int MapId;
	float ticks;
	// Map tiles.
	Group* TileMapGroup;
	Group* GroundEffectGroup;
	Group* DebugIndicatorGroup;
	Group* BulletGroup;
	Group* TowerGroup;
	Group* EnemyGroup;
	Group* EffectGroup;
	Group* UIGroup;
	Group* ItemGroup;
	Engine::Label* UIP1HP;
	Engine::Label* UILives;
	Engine::Label* UIP1ATK;
	Engine::Label* UIP1Range;
	Engine::Label* UIP2HP;
	//Engine::Label* UILives;
	Engine::Label* UIP2ATK;
	Engine::Label* UIP2Range;
	Engine::Label* UIP1CD;
	Engine::Label* UIP2CD;
	Engine::Image* imgP1;
	Engine::Image* imgP2;
	Engine::Image* imgTarget;
	Engine::Image* P1_Iceimg;
	Engine::Image* P2_Iceimg;
	Engine::Image* P1_Shoesimg;
	Engine::Image* P2_Shoesimg;
	Turret* preview;
	std::vector<std::vector<TileType>> mapState;
	std::vector<std::vector<int>> mapDistance;
	std::list<std::pair<int, float>> enemyWaveData;
	std::list<int> keyStrokes;
	static Engine::Point GetClientSize();
	explicit PlayScene() = default;
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
	void OnKeyUp(int keyCode) override;
	void Hit();
	int GetMoney() const;
	void EarnMoney(int money);
	void P1HPUpdate();
	void P1ATKUpdate();
	void P1RangeUpdate();
	void P2HPUpdate();
	void P2ATKUpdate();
	void P2RangeUpdate();
	void AddIceIcon(int ID);
	void AddShoesIcon(int ID);
	void ReadMap();
	void ReadEnemyWave();
	void ConstructUI();
	void UIBtnClicked(int id);
	bool CheckSpaceValid(int x, int y);
	std::vector<std::vector<int>> CalculateBFSDistance();
	// void ModifyReadMapTiles();
	Player1* P1;
	Player2* P2;
	Bomb* bomb[6];
};
#endif // PLAYSCENE_HPP
