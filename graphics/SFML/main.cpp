#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    float rad=100;
    float angle=0.01;
    sf::ConvexShape square;
    square.setPointCount(4);
    square.setPoint(0, sf::Vector2f(-rad, -rad));
    square.setPoint(1, sf::Vector2f(-rad, rad));
    square.setPoint(2, sf::Vector2f(rad, rad));
    square.setPoint(3, sf::Vector2f(rad, -rad));
    square.setFillColor(sf::Color(0,100,100));
    sf::View view(sf::FloatRect(-2*rad,-2*rad, 4*rad, 4*rad));
    window.setView(view);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        square.rotate(angle);
        window.draw(square);


        // end the current frame
        window.display();
    }

    return 0;
}
