#pragma once
#include <Engine.h>
#include "Player.h"

using namespace Engine;

class Level
{
public:
	Level() = default;
	~Level() = default;

	void Init();

	void OnUpdate(Timestep ts);
	void OnRender();
	void OnEvent(Event& e);

	Player& GetPlayer() { return m_Player; }

	bool IsGameOver();

private:
	Player m_Player;

	bool m_GameOver = false;

	Ref<Texture2D> m_RoadTexture;
	Ref<Texture2D> m_GrassTexture;

};