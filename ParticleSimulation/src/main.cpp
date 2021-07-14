#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Particle.h"

void loadFont(std::string path, sf::Font* font);

int main()
{
    const int width = 800, height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "Particle simulation");
    window.setFramerateLimit(60);
    std::map<int, bool> keys;

    float fps;
    sf::Clock clock = sf::Clock::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;
    
    int place = Particle::Type::Sand;
    sf::Font font;
    loadFont("Roboto-Regular.ttf", &font);

    float gravity = 10.f;
    Particle particleSystem(width, height, gravity);
    
    sf::Texture t;
    t.create(window.getSize().x, window.getSize().y);
    sf::Sprite spr(t);

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

        /*
        * update
        */
        if (keys[sf::Keyboard::Escape]) window.close();
        particleSystem.update();

        if (keys[sf::Keyboard::Space])
        {
            place++;
            if (place > Particle::Water)
                place = Particle::Sand;
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
                particleSystem.spawnParticles(20, place, mPos.x, mPos.y);
            }
        }

        /*
        * Drawing
        * first update pixels and sprite/texture
        * then draw
        */
        
        sf::Uint8* pixels = particleSystem.convertToPixels();
        t.update(pixels);
        delete[] pixels;
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

    return 0;
}

void loadFont(std::string path, sf::Font* font)
{
    if (!font->loadFromFile(path))
    {
        std::cout << "Error" << std::endl;
    }
}