#pragma once
#include <SFML/Graphics.hpp>
#include "math.h"

struct Particle_s
{
    unsigned int id;
    bool hasUpdated;
    float maxLifeTime;
    float lifeTime;
    sf::Vector2i velocity;
    sf::Color color;
    float viscosity;
};

class Particle
{
    //public normal functions
public:
    enum Type {Empty, Sand, Water};
    Particle(int width, int height, float gravity);
    ~Particle();
	sf::Uint8* convertToPixels(sf::Uint8* pixels);
    void spawnParticles(int r, int type, int x, int y);
    void update();
    void clearParticles();

    //private normal functions
private:
    math m();
    int width, height;
    float gravity;
    Particle_s* particles;
    void resetParticle(Particle_s* p);
    sf::Color getColor(int type);
    Particle_s* getParticle(int x, int y);
    void setParticle(int x, int y, Particle_s particle);
    Particle_s* moveParticle(Particle_s* curr, int x, int y);
    float getViscosity(int type);


    /*
    * update functions for each type
    */
    void updateSand(int x, int y);
    void updateWater(int x, int y);
};

