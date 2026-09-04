// TriangleSquare.cpp : SFML 3 port of ../graphics_freeglut/TriangleSquare.cpp
// (click 3 points: triangle is drawn, its Heron's-area goes to stdout;
// original window title was the placeholder "Simple GLUT test")

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

float tx[3];
float ty[3];
int i = 0;

// glColor3f(0.5, 0.3, 1)
const sf::Color violet(128, 77, 255);

// the GLUT original works in gluOrtho2D(-300,300,-300,300): px = graph + 300,
// y axis flipped (GL is y-up, SFML is y-down)
sf::Vector2f to_px(float gx, float gy)
{
    return {gx + 300.f, 300.f - gy};
}

// the original's incremental-circle algorithm (GL_LINE_LOOP -> closed LineStrip)
sf::VertexArray buildCircle(float cx, float cy, float r, int num_segments)
{
    const float theta = 2 * 3.1415926f / (float)num_segments;
    const float tangetial_factor = tanf(theta); // calculate the tangential factor
    const float radial_factor = cosf(theta);    // calculate the radial factor

    float x = r; // we start at angle = 0
    float y = 0;

    sf::VertexArray circle(sf::PrimitiveType::LineStrip);
    for (int ii = 0; ii < num_segments; ii++)
    {
        circle.append(sf::Vertex{to_px(x + cx, y + cy), violet});
        const float tx = -y; // tangential vector: flip (x, y) and negate one
        const float ty = x;
        x += tx * tangetial_factor;
        y += ty * tangetial_factor;
        x *= radial_factor;
        y *= radial_factor;
    }
    circle.append(sf::Vertex{to_px(r + cx, 0 + cy), violet}); // close the loop
    return circle;
}

float GetSquare()
{
    const float n1 = sqrt(pow(tx[0] - tx[1], 2) + pow(ty[0] - ty[1], 2));
    const float n2 = sqrt(pow(tx[1] - tx[2], 2) + pow(ty[1] - ty[2], 2));
    const float n3 = sqrt(pow(tx[2] - tx[0], 2) + pow(ty[2] - ty[0], 2));
    const float p = (n1 + n2 + n3) / 2;
    return sqrt(p * (p - n1) * (p - n2) * (p - n3));
}

void drawScene(sf::RenderWindow& window)
{
    // clicked points (5 px dots in GL -> r=2.5)
    for (int j = 0; j < i; j++)
    {
        sf::CircleShape dot(2.5f);
        const sf::Vector2f p = to_px(tx[j], ty[j]);
        dot.setPosition({p.x - 2.5f, p.y - 2.5f});
        dot.setFillColor(violet);
        window.draw(dot);
    }

    if (i == 3)
    {
        sf::VertexArray triangle(sf::PrimitiveType::LineStrip);
        for (int j = 0; j < 3; j++)
            triangle.append(sf::Vertex{to_px(tx[j], ty[j]), violet});
        triangle.append(sf::Vertex{to_px(tx[0], ty[0]), violet}); // close the loop
        window.draw(triangle);
    }

    // the reference circle r=50 around the origin
    window.draw(buildCircle(0, 0, 50, 30));
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Triangle square");
    window.setPosition(sf::Vector2i(700, 100));
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (i < 3)
                {
                    tx[i] = (float)(mousePressed->position.x - 300);
                    ty[i] = (float)(300 - mousePressed->position.y);
                    i++;
                    if (i == 3)
                        std::cout << "Triangle square=" << GetSquare() << std::endl;
                }
            }
        }

        window.clear(sf::Color::White);
        drawScene(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
