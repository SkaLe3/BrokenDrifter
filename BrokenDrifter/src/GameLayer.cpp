#include "pch.h"
#include "GameLayer.h"

#include <glm/gtx/compatibility.hpp>
GameLayer::GameLayer() : Layer()
{
	auto& window = Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());
}

GameLayer::~GameLayer()
{

}

void GameLayer::OnAttach()
{
	m_Level.Init();
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Timestep ts)
{
	EG_PROFILE_FUNCTION();
	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;


	


	switch (m_State)
	{
	case GameState::Play:
	{
		EG_PROFILE_SCOPE("Level.OnUpdate()");
		m_Level.OnUpdate(ts);
		break;
	}
	}
	const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });
	const auto& playerRot = m_Level.GetPlayer().GetRotation();
	if (m_CameraRotation)
		m_Camera->Setrotation(glm::lerp((playerRot - 90.0f), m_Camera->GetRotation(), 0.95f));

	RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Renderer2D::EndScene();
}

void GameLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
	m_Level.OnEvent(event);
}

bool GameLayer::OnWindowResize(WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Key::R) {
		m_CameraRotation = !m_CameraRotation;
		m_Camera->Setrotation(0.0f);
	}
	return true;
}

void GameLayer::CreateCamera(int width, int height)
{
	float aspectRatio = (float)width / (float)height;
	float camHeight = 8.0;
	float bottom = -camHeight;
	float top = camHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}
