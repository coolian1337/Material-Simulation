#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Particle
{
    unsigned int id;
    bool hasUpdated;
    float maxLifeTime;
    float lifeTime;
    sf::Vector2i velocity;
    sf::Vector2i pos;
    sf::Color color;
};

void resetParticle(Particle* p);
void setPixel(int x, int y, sf::Color color, sf::Uint8* pixels, int col);
Particle* getParticle(int x, int y, Particle* particles, int col);
void setParticle(int x, int y, Particle* particles, Particle particle, int col);
int randd();

int main()
{
    const int width = 800, height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "Particle simulation");
    window.setFramerateLimit(300);
    std::map<int, bool> keys;

    enum Type{Empty, Sand};

    float fps;
    sf::Clock clock = sf::Clock::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;

    Particle* particles = new Particle[width * height];
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Particle* p = getParticle(x, y, particles, height);
            p->id = Empty;
            p->color = sf::Color::Black;
            p->hasUpdated = false;
            p->lifeTime = 0.f;
            p->velocity = sf::Vector2i(0, 0);
            p->pos = sf::Vector2i(x, y);
            p->maxLifeTime = -1.f;
        }
    }

    sf::Uint8* pixels = new sf::Uint8[width * height * 4];
    sf::Texture t;
    t.create(window.getSize().x, window.getSize().y);
    sf::Sprite spr(t);

    for (int i = 0; i < width * height * 4; i += 4) {
        pixels[i] = 0;      //r
        pixels[i + 1] = 0;  //g
        pixels[i + 2] = 0;  //b
        pixels[i + 3] = 255;  //a
    }
    t.update(pixels);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) 
            {
            case sf::Event::Closed : window.close();
            case sf::Event::KeyPressed: keys[event.key.code] = true; break;
            case sf::Event::KeyReleased: keys[event.key.code] = false; break;
            default: break;
            }
        }
        for (size_t i = 0; i < width * height * 4; i += 4)
        {
            pixels[i] = 0;      //r
            pixels[i + 1] = 0;  //g
            pixels[i + 2] = 0;  //b
            pixels[i + 3] = 255;  //a
        }

        /*
        * update
        */
        if (keys[sf::Keyboard::Escape]) window.close();
        for (size_t x = 0; x < width; x++)
        {
            for (size_t y = 0; y < height; y++)
            {
                Particle* curr = getParticle(x, y, particles, width);
                if (curr->id == Empty)
                {
                    curr->hasUpdated = true;
                    curr->lifeTime++;
                }
                else if (curr->id == Sand)
                {
                    curr->hasUpdated = true;
                    curr->lifeTime++;
                    if (/*y < height*/ true)
                    {
                        int r = randd();
                        if (getParticle(x, y + 1, particles, width)->id == Empty)
                        {
                            setParticle(x, y + 1, particles, *curr, width);
                            resetParticle(curr);
                        }
                        else if (r == 0)
                        {
                            Particle* left = getParticle(x - 1, y + 1, particles, width);
                            if (left->id == Empty)
                            {
                                setParticle(x - 1, y + 1, particles, *curr, width);
                                resetParticle(curr);
                            }
                        }
                        else {
                            Particle* right = getParticle(x + 1, y + 1, particles, width);
                            if (right->id == Empty)
                            {
                                setParticle(x + 1, y + 1, particles, *curr, width);
                                resetParticle(curr);
                            }
                        }
                    }
                }
            }
        }

        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (mPos.y < 0 || mPos.y > height || mPos.x < 0 || mPos.x > width ? false : true)
            {
                Particle* p = getParticle(mPos.x, mPos.y, particles, width);
                p->id = Sand;
                p->color = sf::Color::Yellow;
            }
        }

        /*
        * Drawing
        * first update pixels and sprite/texture
        * then draw
        */
        for (size_t i = 0; i < width; i++)
        {
            for (size_t j = 0; j < height; j++)
            {
                Particle* p = getParticle(i, j, particles, width);
                setPixel(i, j, p->color, pixels, width);
                p->hasUpdated = false;
            }
        }

        t.update(pixels);
        spr.setTexture(t);
        spr.setPosition(0.0f, 0.0f);

        window.clear();
        window.draw(spr);
        window.display();

        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        std::string f = "Particle simulation fps:";
        f.append(std::to_string(floor(fps)));
        window.setTitle(f);
        previousTime = currentTime;
    }

    delete[] particles;

    return 0;
}

void resetParticle(Particle* p)
{
    p->color = sf::Color::Black;
    p->hasUpdated = false;
    p->id = 0;
    p->lifeTime = 0;
    p->maxLifeTime = -1;
    p->velocity.x = 0;
    p->velocity.y = 0;
}

void setPixel(int x, int y, sf::Color color, sf::Uint8* pixels, int col)
{
    pixels[((y * col + x) * 4) + 0] = color.r;
    pixels[((y * col + x) * 4) + 1] = color.g;
    pixels[((y * col + x) * 4) + 2] = color.b;
    pixels[((y * col + x) * 4) + 3] = color.a;
}

Particle* getParticle(int x, int y, Particle* particles, int col)
{
    return &particles[y * col + x];
}

void setParticle(int x, int y, Particle* particles, Particle particle, int col)
{
    Particle* p = getParticle(x, y, particles, col);
    p->color = particle.color;
    p->id = particle.id;
    p->hasUpdated = particle.hasUpdated;
    p->lifeTime = particle.lifeTime;
    p->maxLifeTime = particle.maxLifeTime;
    p->pos = particle.pos;
    p->velocity = particle.velocity;
}

int randd() {
    return ((rand() / (float)(RAND_MAX + 1)) < 0.5 ? 1 : 0);
}