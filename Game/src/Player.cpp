#include "pch.h"
#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
Player::Player()
{
	m_OneDivideCarMass = 1.0 / m_CarMass;
	m_GroundFriction = m_CarMass * 0.72 * 10.0f;
}

void Player::LoadAssets()
{
	m_Car = Texture2D::Create("assets/textures/Koenigsegg.png");

}

void Player::OnUpdate(Timestep ts)
{

	EG_PROFILE_FUNCTION();
	m_Force.x = 0.0f;
	m_Force.y = 0.0f;
	float speed = glm::length(m_Velocity);
	float angle;

	if (Input::IsKeyPressed(Key::D))
	{
		EG_PROFILE_SCOPE("if (Input::IsKeyPressed(Key::D))");
		angle = -m_MoveDirection * GetSteerFactor(speed) * (180.0f  + 180.0f * m_BrakePressed)* ts ;
		m_Rotation += angle;
		float theta = glm::radians(m_Rotation);
		m_Direction.x = cos(theta);
		m_Direction.y = sin(theta);


	}
	else
		if (Input::IsKeyPressed(Key::A))
		{
			EG_PROFILE_SCOPE("if (Input::IsKeyPressed(Key::A))");


			angle = m_MoveDirection * GetSteerFactor(speed) * 180.0f * ts;
			m_Rotation += angle;
			float theta = glm::radians(m_Rotation);
			m_Direction.x = cos(theta);
			m_Direction.y = sin(theta);

		}

	if (Input::IsKeyPressed(Key::W))
	{
		AddForce(m_Direction * m_EnginePower);
		// Braking when moving backward
		AddForce(-m_Direction * 0.2f * m_GroundFriction * (m_MoveDirection-1));
	}
	else
		if (Input::IsKeyPressed(Key::S))
		{

			AddForce(- m_Direction * m_EnginePower );
			//Braking when moving forward
			AddForce(-m_Direction * 0.2f * m_GroundFriction * (m_MoveDirection + 1));
		}
		else
			if (speed < 0.7f)
			{
				m_Velocity.x = 0.0f;
				m_Velocity.y = 0.0f;
			}

	RecalculateMovement(ts);

	// Apply Air friction
	if (glm::length(m_Velocity) != 0) 
	{ 
		EG_PROFILE_SCOPE("// Apply Air friction");
		m_MoveDirection = glm::dot(glm::normalize(m_Velocity), m_Direction);
		float airFriction =  speed * speed * 0.15f; // 0.15 is air friction factor
		// multiply friction when break presseed;
		AddForce((-glm::normalize(m_Velocity)) * 
			(airFriction +  m_GroundFriction*((1-glm::abs(m_MoveDirection))*(1-m_BrakePressed)) + m_GroundFriction * m_BrakePressed ));
		EG_TRACE("Angle between direction and velocity: ", m_MoveDirection);
	}



	m_Acceleration =  m_Force * m_OneDivideCarMass;
	
	m_Velocity.x += m_Acceleration.x * ts;
	m_Velocity.y += m_Acceleration.y * ts;
		
	m_Position.x += m_Velocity.x * ts;
	m_Position.y += m_Velocity.y * ts;

	std::cout <<"Speed: "<< glm::length(m_Velocity) << std::endl;
	


}

void Player::OnRender()
{
	Renderer2D::DrawRotatedQuad(m_Position, { 1.0f, 1.0f }, glm::radians(m_Rotation - 90.0f), m_Car);
	Renderer2D::DrawQuad(m_Position+ glm::vec3(glm::normalize(m_Velocity), 0.1f), { 0.1f, 0.1f }, { 0.8f, 0.15f, 0.15f, 1.0f });
	Renderer2D::DrawQuad(m_Position + glm::vec3( glm::normalize(m_Direction), 0.0f), { 0.15f, 0.15f }, { 0.15f, 0.15f, 0.8f, 1.0f });
	Renderer2D::DrawQuad(m_Position + glm::vec3(glm::normalize(m_Force), -0.05f), { 0.2f, 0.2f }, { 0.8f, 0.8f, 0.15f, 1.0f });
}

void Player::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));

}

void Player::AddForce(const glm::vec2& force)
{
	m_Force.x += force.x;
	m_Force.y += force.y;

}

void Player::RecalculateMovement(Timestep ts)
{
	EG_PROFILE_FUNCTION();
	float speed = glm::length(m_Velocity);
	float a = speed * 0.01f;
	m_Velocity = glm::lerp( m_Velocity, m_Direction * speed, a );

}

float Player::GetSteerFactor(float speed)
{
	EG_PROFILE_FUNCTION();
	if (speed >= 0 && speed <= 3)
	{
		return 0.46 * glm::sqrt(speed);
	}
	return -0.08f * (speed - 3) + 0.8f;
}

bool Player::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Key::Space)
		m_BrakePressed = 1;
	return true;
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	if (e.GetKeyCode() == Key::Space)
		m_BrakePressed = 0;
	return true;
}
