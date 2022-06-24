#ifndef PLAYERSELECTSCENE2_HPP
#define PLAYERSELECTSCENE2_HPP
#include "IScene.hpp"
#include <allegro5/allegro_audio.h>

class PlayerSelectScene2 final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_INSTANCE* bgmInstance;
public:
	explicit PlayerSelectScene2() = default;
	void Initialize() override;
	void BackOnClick(int stage);
	void OnKeyDown(int keyCode) override;
};

#endif // PLAYERSELECTSCENE2_HPP

