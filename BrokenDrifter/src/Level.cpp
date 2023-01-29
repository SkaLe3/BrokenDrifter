#include "pch.h"
#include "Level.h"

#include "glm/gtc/matrix_transform.hpp"

static bool PointInTri(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}

static bool PointInRect(const glm::vec2& p, const glm::vec2& p0,const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{

	return PointInTri(p, p0, p1, p2) || PointInTri(p, p2, p3, p0);
}



void Level::Init()
{
	m_RoadTexture = Texture2D::Create("assets/textures/Road.png");
	m_RoadCornerTexture = Texture2D::Create("assets/textures/RoadTurn.png");
	m_GrassTexture = Texture2D::Create("assets/textures/Grass.png");


	/*
	* v - vertical road
	* h - horizontal road
	* c - angle road
	* - - rotated corner road -90*
	* + - rotated corner road +90*
	* 8 - rotated corner road  180*
	*		c -
	*		+ 8
	*
	*/
	
	std::string gameMap = "\
...................................\n\
...............chh-                 \n\
...............v..v                 \n\
.chhh-.........v..+hhhhhhhhhh-     \n\
.v...v.........v............c8     \n\
.v...v......c-.+hhhhhh-.....+-     \n\
.v...+hhhhhh8+hhhhhh-.v.....c8     \n\
.v..................v.v.....v      \n\
.v..chhhhhhhhhhhh-..v.v.....v      \n\
.v..+hhhhhhhhhh-.v..v.v	chhh8      \n\
.v..chhhhhhhhhh8.v..v.v	v          \n\
.v..+hhhhhhhhhh-.v..v.v	v          \n\
.v......chhhhhh8.v..+h8	v          \n\
.v......v chhhhhh8......v          \n\
.v......v +hhhhhhhhhhhhh8          \n\
.v......v				           \n\
.v......v				           \n\
.v......v...chhhhhhhh-		       \n\
.v......v...v........v		       \n\
.v......v...v........v		       \n\
.v......v...v........v		       \n\
.v......v...v........v		       \n\
.v......v...v........v		       \n\
.v......v...v........v	           \n\
.v......+hhh8........v		       \n\
c8...................v	           \n\
+hhhhhhhhhhhhhhhhhhhh8	           \n";

										           	

	CreateMap(gameMap);

	m_Player.LoadAssets();
}

void Level::OnUpdate(Timestep ts)
{
	m_Player.OnUpdate(ts);
	auto v = CollisionTest();
	if (v.x != 0.0f || v.y != 0.0f)
	{


		m_Player.OnCollision(ts, v);

	}
}

void Level::OnRender()
{
	Renderer2D::DrawQuad({ -10.0f, -10.0f, -0.2f }, { 300.0f, 300.0f, }, m_GrassTexture, 100);

	for (auto& track : m_Track)
	{
		Renderer2D::DrawRotatedQuad(track->GetPosition(), { 4.0f, 4.0f }, glm::radians(track->GetRotation()), track->GetTexture());
	}

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

void Level::CreateMap(const std::string& gameMap)
{
	float y = 30;
	float x = 0;
	for(int i = 0; i < gameMap.length(); i++, x++)
	{
		if (gameMap[i] == '\n') 
		{
			y--;
			x = 0;
		}

		if (gameMap[i] == 'v')
			m_Track.push_back(CreateRef<StraightTrack>(glm::vec3( x * 4, y * 4, -0.01f) , 0.0f, m_RoadTexture));

		if (gameMap[i] == 'h')
			m_Track.push_back(CreateRef<StraightTrack>(glm::vec3(x * 4, y * 4, -0.01f ), -90.0f, m_RoadTexture));

		if (gameMap[i] == 'c')
			m_Track.push_back(CreateRef<CornerTrack>(glm::vec3(x * 4, y * 4, -0.01f ), 0.0f, m_RoadCornerTexture));

		if (gameMap[i] == '-')
			m_Track.push_back(CreateRef<CornerTrack>(glm::vec3(x * 4, y * 4, -0.01f ), -90.0f, m_RoadCornerTexture));

		if (gameMap[i] == '+')
			m_Track.push_back(CreateRef<CornerTrack>(glm::vec3(x * 4, y * 4, -0.01f ), 90.0f, m_RoadCornerTexture));

		if (gameMap[i] == '8')
			m_Track.push_back(CreateRef<CornerTrack>(glm::vec3(x * 4, y * 4, -0.01f ), 180.0f, m_RoadCornerTexture));
		
	}
}

const glm::vec2 Level::CollisionTest()
{
	
	const auto& pos = m_Player.GetPosition();
	glm::vec4 playerTransformedVerts[4];

	for (int i = 0; i < 4; i++)
	{
		playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f })
			* m_Player.GetCollider()[i];
	}

	for (auto& track : m_Track)
	{
		glm::vec2 corners1[4];
		glm::vec2 corners2[4];
		glm::vec2 normal = { 1.0f, 0.0f };

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(track->GetRotation()), { 0, 0, 1 });
		normal = rotation * glm::vec4(normal, 0.0f, 0.0f);


	
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { track->GetPosition().x, track->GetPosition().y, 0.0f })
			* rotation
			* glm::scale(glm::mat4(1.0f), { 4.0f, 4.0f, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			corners1[i] = transform * track->GetFirstRect()[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInRect({ vert.x, vert.y }, corners1[0], corners1[1], corners1[2], corners1[3]))
			{
				//if (track->GetRotation() == 180.0f )
				//std::cout << normal.x << " " << normal.y << std::endl;
				return normal;
			}
		}

		normal = -normal;
		if (std::dynamic_pointer_cast<CornerTrack>(track)) 
		{
			normal = { 0.0f, -1.0f };
			normal = rotation * glm::vec4(normal, 0.0f, 0.0f);
		}

		
		for (int i = 0; i < 4; i++)
		{
			corners2[i] = transform * track->GetSecondRect()[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInRect({ vert.x, vert.y }, corners2[0], corners2[1], corners2[2], corners2[3])) {
				return normal;
			}
		}
	}
	return {0.0f, 0.0f};

}
