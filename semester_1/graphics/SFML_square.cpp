#include <SFML/Graphics.hpp>


int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    float rad=100;
    const float init_angle_speed=0.01;
    float angle=init_angle_speed;
    sf::VertexArray square(sf::Quads, 4);
    square[0].position=sf::Vector2f(-rad/2, -rad/2);
    square[1].position=sf::Vector2f(-rad/2, rad/2);
    square[2].position=sf::Vector2f(rad/2, rad/2);
    square[3].position=sf::Vector2f(rad/2, -rad/2);
    square[0].color=sf::Color::Red;
    square[1].color=sf::Color::Green;
    square[2].color=sf::Color::Magenta;
    square[3].color=sf::Color::Blue;

    sf::ConvexShape square_rot;
    square_rot.setPointCount(4);
    square_rot.setPoint(0, sf::Vector2f(-rad, -rad));
    square_rot.setPoint(1, sf::Vector2f(-rad, rad));
    square_rot.setPoint(2, sf::Vector2f(rad, rad));
    square_rot.setPoint(3, sf::Vector2f(rad, -rad));
    square_rot.setFillColor(sf::Color(0,100,100));

    sf::View view(sf::FloatRect(-2*rad,-2*rad, 4*rad, 4*rad));
    window.setView(view);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    angle=0;
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    angle=init_angle_speed;
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        square_rot.rotate(angle);
        window.draw(square_rot);
        window.draw(square);

        // end the current frame
        window.display();
    }

    return 0;
}
