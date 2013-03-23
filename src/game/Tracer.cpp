#include "../../include/game/Tracer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

//Defines how often a Points should be add in milliseconds




Tracer::Tracer() : m_timeBetweenPoints(600), m_timeSinceLastAdd(600)
{

}

void Tracer::Reset()
{
    m_timeSinceLastAdd = 0;
    m_points.clear();
}

bool Tracer::Update(float frameTime)
{
    m_timeSinceLastAdd += frameTime;

    if(m_timeSinceLastAdd >= m_timeBetweenPoints)
    {
        return true;
    }

    return false;
}


void Tracer::AddPoint(sf::Vector2f shipPos, bool upsteering, bool speedUp, bool speedDown)
{
    //Set the "timer" to 0
    m_timeSinceLastAdd = 0;

    sf::CircleShape newObject;

    //First change the size and color depending on the parameters
    if(upsteering)
    {
        newObject.setRadius(3);
        newObject.setFillColor(sf::Color::Red);
        newObject.setOutlineColor(sf::Color::White);
        newObject.setOutlineThickness(1);
    }
    else
    {
        newObject.setRadius(3);
        newObject.setFillColor(sf::Color::Red);
    }

    if(speedUp)
    {
        newObject.setFillColor(sf::Color::Blue);
    }

    if(speedDown)
    {
        newObject.setFillColor(sf::Color::Green);
    }

    // Set the middle of the circel to the position
    newObject.setPosition(shipPos.x - newObject.getRadius(), shipPos.y - newObject.getRadius());

    m_points.push_back(newObject);
}

// Draw all the Points
void Tracer::Draw(sf::RenderWindow &window)
{
    for(auto p = m_points.begin(); p != m_points.end(); ++p)
    {
        window.draw(*p);
    }
}


//
// Setter
//

void Tracer::SetTimeBetweenPoints(int milliseconds)
{
    m_timeBetweenPoints = milliseconds;
    m_timeSinceLastAdd = milliseconds+1;
}
