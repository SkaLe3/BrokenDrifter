#pragma once
#include <Engine.h>
/*
* car colider:
* 
* left right bottom top
* 0.29 0.29  0.05   0.05
* 
*/

#include "Level.h"

using namespace Engine;

class GameLayer : public Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event)override;

	bool OnWindowResize(WindowResizeEvent& e);
	bool OnKeyPressed(KeyPressedEvent& e);
private:
	void CreateCamera(int width, int height);
private:
	Scope<OrthographicCamera> m_Camera;
	Level m_Level;
	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState m_State = GameState::Play;
	bool m_CameraRotation = false;

};