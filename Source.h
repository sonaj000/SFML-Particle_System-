#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Particle : public sf::CircleShape
{
public:
	Particle(int radius);

	~Particle();

	void virtual Movement(Particle *to,float dt);

	vector<int>Positions;

	vector<float>Velocities;

	int TotalLife;

	bool bisDead(float dt);

};

class ParticleSystem
{
public:
	ParticleSystem(int nParticles);

	~ParticleSystem();

	std::vector<Particle*> Particles;

	void Randomize(Particle *indP);

};

