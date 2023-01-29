#pragma once
#include <Engine.h>

using namespace Engine;

class Track
{
public:
	Track(const glm::vec3& pos, float rot, Ref<Texture2D> tex)
		: m_Position(pos), m_Rotation(rot), m_TrackTexture(tex) {}
	virtual ~Track() = default;


	virtual const glm::vec4* GetFirstRect() const = 0;
	virtual const glm::vec4* GetSecondRect() const = 0;

	virtual const glm::vec3& GetPosition() const { return m_Position; }
	virtual float GetRotation() const { return m_Rotation; }
	virtual const Ref<Texture2D> GetTexture() const { return m_TrackTexture; }

protected:
	glm::vec3 m_Position;
	float m_Rotation;
	Ref<Texture2D> m_TrackTexture;


};


class StraightTrack : public Track
{
public:
	StraightTrack(const glm::vec3& pos, float rot, Ref<Texture2D> tex)
		: Track(pos, rot, tex) {}
	
	virtual const glm::vec4* GetFirstRect() const { return FirstRect; }
	virtual const glm::vec4* GetSecondRect() const { return SecondRect; }

private:
	glm::vec4 FirstRect[4] = {
			{-0.5f         , -0.5f, 0.0f, 1.0f},
			{-0.5f + 0.026f, -0.5f, 0.0f, 1.0f},
			{-0.5f + 0.026f,  0.5f, 0.0f, 1.0f},
			{-0.5f         ,  0.5f, 0.0f, 1.0f}
							};
	glm::vec4 SecondRect[4] = {
			{0.5f         ,	-0.5f, 0.0f, 1.0f},
			{0.5f + 0.026f, -0.5f, 0.0f, 1.0f},
			{0.5f + 0.026f,  0.5f, 0.0f, 1.0f},
			{0.5f         ,	0.5f, 0.0f, 1.0f}
								};

};

class CornerTrack : public Track
{
public:
	CornerTrack(const glm::vec3& pos, float rot, Ref<Texture2D> tex)
		: Track(pos, rot, tex) {}

	virtual const glm::vec4* GetFirstRect() const { return FirstRect; }
	virtual const glm::vec4* GetSecondRect() const { return SecondRect; }
private:

	glm::vec4 FirstRect[4] = {
			{-0.5f         , -0.5f, 0.0f, 1.0f},
			{-0.5f + 0.026f, -0.5f, 0.0f, 1.0f},
			{-0.5f + 0.026f,  0.5f, 0.0f, 1.0f},
			{-0.5f         ,  0.5f, 0.0f, 1.0f}
	};
	glm::vec4 SecondRect[4] = {
			{-0.5f,	 0.5f - 0.026f, 0.0f, 1.0f},
			{0.5f ,  0.5f - 0.026f, 0.0f, 1.0f},
			{0.5f ,  0.5f         , 0.0f, 1.0f},
			{-0.5f,	 0.5f         , 0.0f, 1.0f}
	};

};

