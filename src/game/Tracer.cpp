#include "../../include/game/Tracer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

//Defines how often a Points should be add in milliseconds
const static int TIME_BETWEEN_POINTS = 200;



Tracer::Tracer() : m_timeSinceLastAdd(0)
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

    if(m_timeSinceLastAdd >= TIME_BETWEEN_POINTS)
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
        newObject.setRadius(5);
        newObject.setFillColor(sf::Color::Red);
        newObject.setOutlineColor(sf::Color::White);
        newObject.setOutlineThickness(2);
    }
    else
    {
        newObject.setRadius(5);
        newObject.setFillColor(sf::Color::Red);
    }

    if(speedUp)
    {
        newObject.setFillColor(sf::Color::Blue);
    }

    if(speedDown)
    {
        newObject.setFillColor(sf::Color::Yellow);
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
