#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>

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
void spawnParticles(int r, Particle* particles, int type, int x, int y, int width);
void loadFont(std::string path, sf::Font* font);
sf::Color getColor(int type);
float calcDist(float x, float y);
int randd();

int main()
{
    const int width = 800, height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "Particle simulation");
    window.setFramerateLimit(600);
    std::map<int, bool> keys;

    enum Type{Empty, Sand, Water};

    float fps;
    sf::Clock clock = sf::Clock::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;

    int place = Sand;
    sf::Font font;
    loadFont("Roboto-Regular.ttf", &font);

    Particle* particles = new Particle[width * height];
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Particle* p = getParticle(x, y, particles, width);
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
                if (curr->hasUpdated != true)
                {
                    if (curr->id == Empty)
                    {
                        curr->hasUpdated = true;
                        curr->lifeTime++;
                    }
                    else if (curr->id == Sand)
                    {
                        curr->hasUpdated = true;
                        curr->lifeTime++;
                        if (y + 1 < height)
                        {
                            if (getParticle(x, y + 1, particles, width)->id == Empty)
                            {
                                setParticle(x, y + 1, particles, *curr, width);
                                resetParticle(curr);
                            }
                            else if (getParticle(x - 1, y + 1, particles, width)->id == Empty)
                            {
                                Particle* left = getParticle(x - 1, y + 1, particles, width);
                                if (left->id == Empty)
                                {
                                    setParticle(x - 1, y + 1, particles, *curr, width);
                                    resetParticle(curr);
                                }
                            }
                            else if (getParticle(x + 1, y + 1, particles, width)->id == Empty) {
                                Particle* right = getParticle(x + 1, y + 1, particles, width);
                                if (right->id == Empty)
                                {
                                    setParticle(x + 1, y + 1, particles, *curr, width);
                                    resetParticle(curr);
                                }
                            }
                        }
                    }
                    else if (curr->id == Water)
                    {
                        curr->hasUpdated = true;
                        curr->lifeTime++;
                        if (y + 1 < height)
                        {
                            if (getParticle(x, y + 1, particles, width)->id == Empty)
                            {
                                setParticle(x, y + 1, particles, *curr, width);
                                resetParticle(curr);
                            }
                            else if (getParticle(x - 1, y + 1, particles, width)->id == Empty)
                            {
                                Particle* left = getParticle(x - 1, y + 1, particles, width);
                                if (left->id == Empty)
                                {
                                    setParticle(x - 1, y + 1, particles, *curr, width);
                                    resetParticle(curr);
                                }
                            }
                            else if (getParticle(x + 1, y + 1, particles, width)->id == Empty) {
                                Particle* right = getParticle(x + 1, y + 1, particles, width);
                                if (right->id == Empty)
                                {
                                    setParticle(x + 1, y + 1, particles, *curr, width);
                                    resetParticle(curr);
                                }
                            }
                            else if (getParticle(x + 1, y, particles, width)->id == Empty)
                            {
                                Particle* right = getParticle(x + 1, y, particles, width);
                                if (right->id == Empty)
                                {
                                    setParticle(x + 1, y, particles, *curr, width);
                                    resetParticle(curr);
                                }
                            }
                            else if (getParticle(x - 1, y, particles, width)->id == Empty)
                            {
                                Particle* right = getParticle(x - 1, y, particles, width);
                                if (right->id == Empty)
                                {
                                    setParticle(x - 1, y, particles, *curr, width);
                                    resetParticle(curr);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (keys[sf::Keyboard::Space])
        {
            place++;
            if (place > Water)
                place = Sand;
        }

        sf::Text te;
        te.setPosition(750, 10);
        te.setFont(font);
        te.setFillColor(sf::Color::White);
        te.setCharacterSize(20);
        te.setString(std::to_string(place));

        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (mPos.y < 0 || mPos.y > height || mPos.x < 0 || mPos.x > width ? false : true)
            {
                spawnParticles(5, particles, place, mPos.x, mPos.y, width);
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

        window.clear();
        window.draw(spr);
        window.draw(te);
        window.display();

        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        std::string f = "Particle simulation fps:";
        f.append(std::to_string(floor(fps)));
        window.setTitle(f);
        previousTime = currentTime;
    }

    delete[] particles;
    delete[] pixels;

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

void spawnParticles(int r, Particle* particles, int type, int x, int y, int width)
{
    std::vector<sf::Vector2i> positions = std::vector<sf::Vector2i>();
    int s = powf(r, 2);
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            int dx = calcDist(i, j);
            if (dx <= r)
                positions.push_back(sf::Vector2i(i, j));
        }
    }

    for (int i = 0; i < positions.size(); i++)
    {
        Particle* p = getParticle(positions.at(i).x, positions.at(i).x, particles, width);
        p->id = type;
        p->color = getColor(type);
        p->hasUpdated = true;
        p->pos = positions.at(i);
    }
}

void loadFont(std::string path, sf::Font* font)
{
    if (!font->loadFromFile(path))
    {
        std::cout << "Error" << std::endl;
    }
}

sf::Color getColor(int type)
{
    switch (type) 
    {
    case 0: return sf::Color::Black;
    case 1: return sf::Color::Yellow;
    case 2: return sf::Color::Blue;
    }
}

float calcDist(float x, float y)
{
    return sqrtf(powf(x - x, 2) + powf(y - y, 2));
}

int randd() {
    return ((rand() / (float)(RAND_MAX + 1)) < 0.5 ? 1 : 0);
}