#include "Particle.h"
#include "math.h"

Particle::Particle(int width, int height, float gravity)
{
    this->height = height;
    this->width = width;
    this->gravity = gravity;
    particles = new Particle_s[width * height];
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Particle_s* p = getParticle(x, y);
            p->id = Empty;
            p->color = sf::Color::Black;
            p->hasUpdated = false;
            p->lifeTime = 0.f;
            p->velocity = sf::Vector2i(0, 0);
            p->maxLifeTime = -1.f;
        }
    }
}

Particle::~Particle()
{
    delete[] particles;
}

sf::Uint8* Particle::convertToPixels()
{
    sf::Uint8* pixels = new sf::Uint8[width * height * 4];
    for (int i = width - 1; i > 0; i--)
    {
        for (int j = height - 1; j > 0; j--)
        {
            Particle_s* p = getParticle(i, j);
            pixels[((j * width + i) * 4) + 0] = p->color.r;
            pixels[((j * width + i) * 4) + 1] = p->color.g;
            pixels[((j * width + i) * 4) + 2] = p->color.b;
            pixels[((j * width + i) * 4) + 3] = p->color.a;
            p->hasUpdated = false;
        }
    }
    return pixels;
}

void Particle::spawnParticles(int r, int type, int x, int y)
{
    x = x - r / 2;
    y = y - r / 2;
    std::vector<sf::Vector2i> positions = std::vector<sf::Vector2i>();
    int s = r * r;
    int c = 0;
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            float dx = m.calcDist(x + (i - r), y + (j - r), x, y);
            if (dx <= r)
            {
                if (c % 2 == 0)
                    positions.push_back(sf::Vector2i(i, j));

                c++;
            }
        }
    }

    for (int i = 0; i < positions.size(); i++)
    {
        Particle_s* p = getParticle(x + positions.at(i).x, y + positions.at(i).y);
        p->id = type;
        p->color = getColor(type);
        p->hasUpdated = true;
        p->viscosity = getViscosity(type);
    }
}

void Particle::update()
{
    for (int x = width - 1; x > 0; x--)
    {
        for (int y = height - 1; y > 0; y--)
        {
            Particle_s* curr = getParticle(x, y);
            if (curr->hasUpdated != true)
            {
                if (curr->id != Empty)
                {
                    curr->hasUpdated = true;
                    curr->lifeTime++;

                    switch (curr->id)
                    {
                    case Sand: updateSand(x, y); break;
                    case Water: updateWater(x, y); break;
                    }
                }
            }
        }
    }
}

void Particle::resetParticle(Particle_s* p)
{
    p->color = sf::Color::Black;
    p->hasUpdated = false;
    p->id = Empty;
    p->lifeTime = 0;
    p->maxLifeTime = -1;
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->viscosity = 0.0f;
}

float Particle::getViscosity(int type) 
{
    switch (type)
    {
    case 0: return 0.0f;
    case 1: return 0.0f;
    case 2: return 7.0f;
    }
}

sf::Color Particle::getColor(int type)
{
    switch (type)
    {
    case 0: return sf::Color::Black;
    case 1: return sf::Color::Yellow;
    case 2: return sf::Color::Blue;
    }
}

Particle_s* Particle::getParticle(int x, int y)
{
    return &particles[y * width + x];
}

void Particle::moveParticle(Particle_s* curr,int x, int y)
{
    Particle_s* loc = getParticle(x, y);
    memcpy(loc, curr, sizeof(Particle_s));
    resetParticle(curr);
}

void Particle::updateSand(int x, int y)
{
    Particle_s* curr = getParticle(x, y);
    curr->velocity.y = gravity;
    int cX = x;
    int cY = y;
    
    for (int i = 0; i < curr->velocity.y; i++)
    {
        if (cY + 1 < height)
        {
            Particle_s* p = getParticle(cX, cY += 1);

            if (p->id == Empty)
            {
                moveParticle(curr, cX, cY);
                curr->velocity.y -= 1;
            }
            else
            {
                p = getParticle(cX += 1, cY);
                if (p->id == Empty && curr->velocity.y > 0)
                {
                    moveParticle(curr, cX, cY);
                    curr->velocity.y -= 1;
                }
                else
                {
                    p = getParticle(cX -= 2, cY);
                    if (p->id == Empty && curr->velocity.y > 0)
                    {
                        moveParticle(curr, cX, cY);
                        curr->velocity.y -= 1;
                    }
                }
            }
            curr = &*p;
        }
    }
}

void Particle::updateWater(int x, int y)
{
    Particle_s* curr = getParticle(x, y);
    curr->velocity.y = gravity * curr->viscosity;
    int cX = x;
    int cY = y;

    for (int i = 0; i < curr->velocity.y; i++)
    {
        int dir = m.randd();
        if (cY + 1 < height)
        {
            if (getParticle(cX, cY + 1)->id == Empty)
            {
                moveParticle(curr, cX, cY + 1);
                curr = getParticle(cX, cY + 1);
                curr->velocity.y -= 1;
                cY += 1;
            }
            else if (getParticle(cX + 1, cY + 1)->id == Empty && dir == 0)
            {
                moveParticle(curr, cX + 1, cY + 1);
                curr = getParticle(cX + 1, cY + 1);
                curr->velocity.y -= 1;
                cX += 1;
                cY += 1;
            }
            else if(getParticle(cX - 1, cY + 1)->id == Empty && dir == 1)
            {
                moveParticle(curr, cX - 1, cY + 1);
                curr = getParticle(cX - 1, cY + 1);
                curr->velocity.y -= 1;
                cY += 1;
                cX -= 1;
            }
            else if (getParticle(cX + 1, cY)->id == Empty && dir == 0)
            {
                moveParticle(curr, cX + 1, cY);
                curr = getParticle(cX + 1, cY);
                curr->velocity.y -= 1;
                cX += 1;
            }
            else if(getParticle(cX - 1, cY)->id == Empty && dir == 1)
            {
                moveParticle(curr, cX - 1, cY);
                curr = getParticle(cX - 1, cY);
                curr->velocity.y -= 1;
                cX -= 1;
            }
        }
    }
}

void Particle::setParticle(int x, int y, Particle_s particle)
{
    Particle_s* p = getParticle(x, y);
    memcpy(p, &particle, sizeof(Particle_s));
}
