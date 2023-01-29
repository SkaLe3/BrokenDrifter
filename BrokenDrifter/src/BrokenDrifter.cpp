#include <Engine.h>
#include <Engine/Core/EntryPoint.h>
#include <iostream>

#include "glm/glm/glm.hpp"

#include "GameLayer.h"




class StillAliveApp : public  Engine::Application
{
public:
	StillAliveApp()
	{
		//Application::Get().GetWindow().SetVSync(false);
		PushLayer(new GameLayer());
	}
	~StillAliveApp()
	{

	}

private:
	


};

Engine::Application* Engine::CreateApplication()
{
	return new StillAliveApp();
}