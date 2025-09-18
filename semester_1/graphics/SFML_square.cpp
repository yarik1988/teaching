#include <SFML/Graphics.hpp>


int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800,800), 800), "My window");
    constexpr float side_len=100;
    constexpr float init_angle_speed=0.01;
    float angle=init_angle_speed;
    sf::VertexArray square(sf::PrimitiveType::Triangles, 6);
    square[0].position=sf::Vector2f(-side_len/2, -side_len/2);
    square[1].position=sf::Vector2f(-side_len/2, side_len/2);
    square[2].position=sf::Vector2f(side_len/2, side_len/2);
    square[3].position=sf::Vector2f(-side_len/2, -side_len/2);
    square[4].position=sf::Vector2f(side_len/2, -side_len/2);
    square[5].position=sf::Vector2f(side_len/2, side_len/2);
    square[0].color=sf::Color::Red;
    square[1].color=sf::Color::Green;
    square[2].color=sf::Color::Magenta;
    square[3].color=sf::Color::Red;
    square[4].color=sf::Color::Blue;
    square[5].color=sf::Color::Magenta;

    sf::View view(sf::FloatRect(sf::Vector2f(-side_len,-side_len),sf::Vector2f(2*side_len, 2*side_len)));
    window.setView(view);
    sf::Transform rotation;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                    angle=0;
            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                    angle=init_angle_speed;
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        rotation.rotate(sf::degrees(angle));
        window.draw(square,rotation);
        window.display();
    }

    return 0;
}
