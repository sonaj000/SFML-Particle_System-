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
#include <queue>

using namespace std;


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	queue<ParticleSystem*>Manage;


	bool alreadyClicked = false;


	sf::Vector2i localPosition = { 0,0 };
	//clock
	sf::Clock mClock;
	mClock.restart();

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !alreadyClicked)
		{
			cout << "made anew particle" << "\n";
			ParticleSystem* System = new ParticleSystem(20);//allocation
			Manage.push(System);
			System->bcanSpawn = true;
			System->PTime.restart();
			localPosition = sf::Mouse::getPosition(window);
			for (Particle* ind : System->Particles)
			{
				ind->setPosition(float(localPosition.x), float(localPosition.y));
			}
			alreadyClicked = true;
		}
		if (alreadyClicked && mClock.getElapsedTime().asSeconds() > 2.0f)
		{
			alreadyClicked = false;
			mClock.restart();
			cout << "Reset" << "\n";
		}
		if (!Manage.empty())
		{
			ParticleSystem* SH = Manage.front();
			if (SH && SH->bcanSpawn)
			{
				float deltatime = SH->PTime.getElapsedTime().asSeconds();
				//cout << deltatime << "\n";
				//start the timer here. 
				for (Particle* ind : SH->Particles)
				{
					if (ind != nullptr)
					{
						ind->Movement(ind, deltatime);
						window.draw(*ind);
						//cout << "drawing" << "\n";
					}
				}
				//alreadyClicked = false;
				for (int i{ 0 }; i < SH->Particles.size(); i++)
				{
					Particle* h = SH->Particles[i];
					if (deltatime > h->TotalLife)
					{
						delete h; //this is probably unncesary since i;m not allocating anything haha
						h = nullptr;
						SH->Particles.erase(SH->Particles.begin() + i);
						//cout << "erasing" << "\n";
					}
				}
				if (SH->Particles.size() < 1)
				{
					delete SH; //free memory 
					SH = nullptr;
					cout << "true" << "\n";
					Manage.pop();
				}
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
	to->move(sf::Vector2f(dt * to->Velocities[0] * 0.3, dt * 0.3 * to->Velocities[1]));
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
	bcanSpawn = false;

}

ParticleSystem::~ParticleSystem()
{
	for (Particle* particle : Particles) 
	{
		delete particle;
	}
}

void ParticleSystem::Randomize(Particle* indP)
{
	int color = rand() % 5;
	//cout << color << "\n";
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
