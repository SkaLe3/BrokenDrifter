#pragma once
#include <Engine.h>

#include <glm/glm.hpp>

using namespace Engine;

class Player
{
public:
	Player();
	~Player() = default;

	void LoadAssets();

	void OnUpdate(Timestep ts);
	void OnRender();
	void OnEvent(Event& e);

	glm::vec3& GetPosition() { return m_Position; }
	float GetRotation() { return m_Rotation; }
private:
	void AddForce(const glm::vec2& force);
	void RecalculateMovement(Timestep ts);
	float GetSteerFactor(float speed);

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 90.0f;
	glm::vec2 m_Direction = { 0.0f, 1.0f };
	float m_MoveDirection = 1.0f;

	glm::vec2 m_Velocity = { 0.0f, 0.0f };

	glm::vec2 m_Force = { 0.0f, 0.0f };
	float m_EnginePower = 15.0f;
	glm::vec2 m_Acceleration = { 0.0f, 0.0f };
	float m_CarMass = 4.0f;
	float m_OneDivideCarMass;
	float m_GroundFriction = 0.0f;
	int m_BrakePressed = 0;

	Ref<Texture2D> m_Car;
};