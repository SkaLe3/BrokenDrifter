#pragma once
#include <Engine.h>
#include "Player.h"
#include "Track.h"

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
	void CreateMap(const std::string& gameMap);
	const glm::vec2 CollisionTest();

private:
	Player m_Player;
	std::vector<Ref<Track>> m_Track;
	bool m_GameOver = false;

	Ref<Texture2D> m_RoadTexture;
	Ref<Texture2D> m_RoadCornerTexture;
	Ref<Texture2D> m_GrassTexture;

};


