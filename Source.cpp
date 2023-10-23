#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <SFML/System/Clock.hpp>
#include "Source.h"
#include <iostream>
#include <random>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	Particle* holder = new Particle(50);
	holder->setFillColor(sf::Color::Red);

	sf::Clock Timer;
	Timer.restart();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		float deltatime = Timer.getElapsedTime().asSeconds();
		window.clear(sf::Color::Black);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			cout << "button pressed" << "\n";
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			ParticleSystem* System = new ParticleSystem(5);
			for (Particle* ind : System->Particles)
			{
				ind->setPosition(float(localPosition.x), float(localPosition.y));
				ind->Movement(ind, deltatime);
				window.draw(*ind);
				if (deltatime > ind->TotalLife)
				{
					window.clear(sf::Color::Black);
					//delete ind;
					//ind = nullptr;
				}
			}
		}

		//delete system after 
		//if (holder)
		//{
		//	holder->Movement(holder, deltatime, 0.01f);
		//	window.draw(*holder);
		//	if (deltatime > 3.0f)
		//	{
		//		window.clear(sf::Color::Black);
		//		delete holder;
		//		holder = nullptr;
		//	}
		//}
		window.display();
	}
	return 0;
}

Particle::Particle(int radius) : sf::CircleShape(radius)
{
	Positions = { 0,0 };
	Velocities = { 0,0 };
	TotalLife = 3;
}

Particle::~Particle()
{
}

void Particle::Movement(Particle *to, float dt)
{
	//cout << dt << "\n";
	to->move(sf::Vector2f(dt * to->Velocities[0] * 0.1, dt * 0.1 * to->Velocities[1]));

}


ParticleSystem::ParticleSystem(int nParticles)
{
	for (int i{ 0 }; i < nParticles; i++)
	{
		int random_size = rand() % 100;
		Particle* P = new Particle(random_size);
		Randomize(P);
		Particles.push_back(P);
	}

}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Randomize(Particle* indP)
{
	int color = rand() % 5;
	cout << color << "\n";
	switch (color) //randomize color
	{
		case 0:
			indP->setFillColor(sf::Color::Red);
			break;
		case 1:
			indP->setFillColor(sf::Color::Yellow);
			break;
		case 2:
			indP->setFillColor(sf::Color::Green);
			break;
		case 3:
			indP->setFillColor(sf::Color::Blue);
			break;
		default:
			indP->setFillColor(sf::Color::White);
			break;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	float randomFloatX  = dis(gen);
	float randomFloatY = dis(gen);
	indP->Velocities = { randomFloatX,randomFloatY}; // randomize velocity for x and y
	indP->TotalLife = rand() % 5; // randomize lifetime of particle
}
