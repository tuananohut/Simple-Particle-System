#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

class Particle
{
public:
    Particle()
    {
        ellipse = std::make_shared<sf::CircleShape>();
        ellipse->setPosition(this->x, this->y);
        ellipse->setRadius(16.0f);
        ellipse->setOrigin(ellipse->getRadius(), ellipse->getRadius());
        ellipse->setFillColor(sf::Color(255, 255, 255, this->alpha));
    }

    float x = 400.0f, y = 580.0f;

    float vx = generateRandomFloat(-2.0f, 2.0f);
    float vy = generateRandomFloat(-10.0f, -1.0f);

    float alpha = 255;

    void update()
    {
        this->x += this->vx;
        this->y += this->vy;

        this->alpha -= 5;

        ellipse->setFillColor(sf::Color(255, 255, 255, alpha));
        ellipse->setPosition(sf::Vector2f(x, y));
    }

    float generateRandomFloat(float min, float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    std::shared_ptr<sf::CircleShape> ellipse;
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simple Particle System", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    std::vector<std::shared_ptr<Particle>> particles;

    while (window.isOpen())
    {
        const auto start{ std::chrono::steady_clock::now() };

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
 
        for (int i = 0; i < 5; i++)
        {
            std::shared_ptr<Particle> particle = std::make_shared<Particle>();
            particles.push_back(particle);
        }
        
        for (int i = particles.size() - 1; i >=0; i--)
        {
            particles[i]->update();
            if(particles[i]->alpha <= 0)
            {
                particles.erase(std::remove(particles.begin(), particles.end(), particles[i]), particles.end());
            }
            window.draw(*particles[i]->ellipse);
        }

        window.display();

        const auto end{ std::chrono::steady_clock::now() };
        const std::chrono::duration<double> elapsed_seconds{ end - start };
        sf::sleep(sf::milliseconds(static_cast<int>((1.0 / 60.0 - elapsed_seconds.count()) * 1000)));
    }

    return 0;
}
