#include "pch.h"
#include "Level.h"

void Level::Init()
{
	m_RoadTexture = Texture2D::Create("assets/textures/Road.png");
	m_GrassTexture = Texture2D::Create("assets/textures/Grass.png");

	m_Player.LoadAssets();
}

void Level::OnUpdate(Timestep ts)
{
	m_Player.OnUpdate(ts);
}

void Level::OnRender()
{
	Renderer2D::DrawQuad({ -10.0f, -10.0f, -0.2f }, { 100.0f, 100.0f, }, m_GrassTexture, 100);
	for (int i = 0; i < 20; i++)
		Renderer2D::DrawQuad({ 0.0f, -32.0f + i*4.0f, -0.1f }, { 4.0f, 4.0f }, m_RoadTexture);

	for (int i = 0; i < 20; i++)
		Renderer2D::DrawQuad({ 8.0f, -32.0f + i * 4.0f, -0.1f }, { 4.0f, 4.0f }, m_RoadTexture);

	for (int i = 0; i < 20; i++)
		Renderer2D::DrawQuad({ -8.0f, -32.0f + i * 4.0f, -0.1f }, { 4.0f, 4.0f }, m_RoadTexture);

	

	m_Player.OnRender();
}

void Level::OnEvent(Event& e)
{
	m_Player.OnEvent(e);
}

bool Level::IsGameOver()
{
	return false;
}
