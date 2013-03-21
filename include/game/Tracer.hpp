#ifndef TRACER_HPP
#define TRACER_HPP

#include<SFML/Graphics/CircleShape.hpp>

namespace sf
{
    class RenderWindow;
}

// Description
//
// This class will save the position + status of the ship when then AddPointFunction is called.
// Then you can draw it and display the way the ship traveled
//
class Tracer
{
public:

    Tracer();


    //This function will reset the vector
    void Reset();

    //Check this on every frame
    //Param
    //frameTime = in milliseconds
    //Return
    //True if there the var m_timeSinceLastAdd is bigger then TIME_BETWEEN_POINTS
    bool Update(float frameTime);


    // Add a Point to the tracer
    // Param
    // upsteering = is space pressed?
    // speedUp    = right arrowkey pressed?
    // speedDown  = left arrowKey pressed?
    void AddPoint(sf::Vector2f shipPos, bool upsteering, bool speedUp, bool speedDown);


    // Draws the Tracerline
    void Draw(sf::RenderWindow &window);

private:

private:

    //
    std::vector<sf::CircleShape> m_points;

    float m_timeSinceLastAdd;

};

#endif // TRACER_HPP
