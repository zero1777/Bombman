#ifndef PLAYERSELECTSCENE_HPP
#define PLAYERSELECTSCENE_HPP
#include "IScene.hpp"
#include <allegro5/allegro_audio.h>

class PlayerSelectScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_INSTANCE* bgmInstance;
public:
	explicit PlayerSelectScene() = default;
	void Initialize() override;
	void BackOnClick(int stage);
	void OnKeyDown(int keyCode) override;
};

#endif // PLAYERSELECTSCENE_HPP
