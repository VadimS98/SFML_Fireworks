#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include "windows.h"

#define ScreenHeigth 1024
#define ScreenWeight 512
#define FireworksAmount 5


class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
	sf::Color m_transparent;
    ParticleSystem(unsigned int count) :
    m_particles(count),
    m_vertices(sf::Points, count),
    m_lifetime(sf::seconds(3.f)),
    m_emitter(0.f, 0.f)
    {
    }

    void setEmitter(sf::Vector2f position)
    {
        m_emitter = position;
    }

    void update(sf::Time elapsed, int randomInt1, int randomInt2, int randomInt3)
    {
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            // if the particle is dead, respawn it
            if (p.lifetime <= sf::Time::Zero)
                resetParticle(i);
            m_vertices[i].position += p.velocity * elapsed.asSeconds();
            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            m_vertices[i].color.r = static_cast<sf::Uint8>(randomInt1);
            m_vertices[i].color.g = static_cast<sf::Uint8>(randomInt2);
            m_vertices[i].color.b = static_cast<sf::Uint8>(randomInt3);
            m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
        }
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = NULL;
        target.draw(m_vertices, states);
    }

private:

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void resetParticle(std::size_t index)
    {
        float angle = (std::rand() % 360) * 3.14f / 180.f;
        float speed = (std::rand() % 50) + 50.f;
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);
        m_vertices[index].position = m_emitter;
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetime;
    sf::Vector2f m_emitter;
};

sf::Vector2f randomPoints() {
	return sf::Vector2f(rand() % ScreenHeigth + 1, rand() % ScreenWeight + 1);
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(ScreenHeigth, ScreenWeight), "Particles");
    ParticleSystem particles(50000);

    sf::Clock clock;
    sf::Clock newFirework;
    sf::Vector2f points[FireworksAmount];
    int random1 = 125, random2 = 0, random3 = 255;

    // run the main loop
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            	// Add Mouse Event(RightNowDontDetectClick)
            }

        }


        if (newFirework.getElapsedTime() > sf::seconds(2)) {
        	for (int i = 0; i < FireworksAmount; i++) {
        		points[i] = randomPoints();
        	}
        	random1 = rand() % 255 + 1;
        	random2 = rand() % 255 + 1;
        	random3 = rand() % 255 + 1;
        	newFirework.restart();
        }

        for (int i = 0; i < FireworksAmount; i++) {
			particles.setEmitter(points[i]);
			sf::Time elapsed = clock.restart();
			particles.update(elapsed, random1, random2, random3);

        }
        window.clear();
        window.draw(particles);
        window.display();
    }

    return 0;
}
