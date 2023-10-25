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

	ParticleSystem* System = nullptr;
	sf::Clock Timer;
	Timer.restart();
	// to have multiple particle systems going at once, we're going need to instantiate a timer per particle system so it can run independently based on my logic. 
	// ideally i'll implement this as a class variable

	sf::Vector2i localPosition = { 0,0 };
	bool bcanSpawn = false;

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
			if (!bcanSpawn)
			{
				cout << "bcanspawn" << "\n";
				if (System == nullptr)
				{
					cout << "system is null" << "\n";
					System = new ParticleSystem(20);//allocation
					localPosition = sf::Mouse::getPosition(window);
					for (Particle* ind : System->Particles)
					{
						ind->setPosition(float(localPosition.x), float(localPosition.y));
						cout << "position set" << "\n";
					}
				}
				bcanSpawn = true;
			}
		}
		if (bcanSpawn && System)
		{
			cout << "bspawn set false" << "\n";
			for (Particle* ind : System->Particles)
			{
				if (ind != nullptr)
				{
					ind->Movement(ind, deltatime);
					window.draw(*ind);
					cout << "drawing" << "\n";
				}
			}
			for (int i{ 0 }; i < System->Particles.size(); i++)
			{
				Particle* h = System->Particles[i];
				if (deltatime > h->TotalLife)
				{
					delete h; //this is probably unncesary since i;m not allocating anything haha
					h = nullptr;
					System->Particles.erase(System->Particles.begin() + i);
					cout << "erasing" << "\n";
				}
			}
			if (System->Particles.size() < 1)
			{
				delete System; //free memory 
				System = nullptr;
				cout << "true" << "\n";
				bcanSpawn = false;
				Timer.restart(); // need to reset timer otherwise we balling
			}
		}

		window.display();
	}
	return 0;
}

Particle::Particle(int radius) : sf::CircleShape(radius)
{
	//these are irrelevant and will be randomized or set later
	Positions = { 300,300 };
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
	if (dt > to->TotalLife)
	{
		dt = 0.0f;
		return;
	}
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
	indP->TotalLife = rand() % (5 - 1 + 1) + 1; // randomize lifetime of particle but set to 3 rn for debugging purposes rand() % (max - min + 1) + min;
	//indP->setPosition(300, 300); // set to 300,300 rn for debugging purposes but will change to base it on mouse position. 
}
