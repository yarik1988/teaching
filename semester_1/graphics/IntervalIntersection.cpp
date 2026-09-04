// IntervalIntersection.cpp : SFML 3 port of ../graphics_freeglut/IntervalIntersection.cpp
// (click 4 points: two segments are drawn and their intersection is computed;
// the result also goes to stdout like in the original, which printed 0 or 1;
// original window title was the placeholder "Simple GLUT test")

#include <SFML/Graphics.hpp>
#include <iostream>

int tx[4];
int ty[4];
int xx, yy;
int i = 0;
bool intersected = false;

// the original integer-math line-line intersection, unchanged
bool isIntersect()
{
    int a1 = ty[1] - ty[0];
    int b1 = tx[0] - tx[1];
    int c1 = ty[0] * tx[1] - ty[1] * tx[0];
    int a2 = ty[3] - ty[2];
    int b2 = tx[2] - tx[3];
    int c2 = ty[2] * tx[3] - ty[3] * tx[2];
    if ((a1 * b2 - a2 * b1) == 0) return false;
    xx = -(c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
    yy = -(a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1);

    bool t1, t2, t3, t4;
    t1 = ((xx <= tx[1] && xx >= tx[0]) || (xx <= tx[0] && xx >= tx[1]));
    t2 = ((xx <= tx[3] && xx >= tx[2]) || (xx <= tx[2] && xx >= tx[3]));
    t3 = ((yy <= ty[1] && yy >= ty[0]) || (yy <= ty[0] && yy >= ty[1]));
    t4 = ((yy <= ty[3] && yy >= ty[2]) || (yy <= ty[2] && yy >= ty[3]));

    return t1 && t2 && t3 && t4;
}

// the GLUT original works in gluOrtho2D(-300,300,-300,300): px = graph + 300,
// y axis flipped (GL is y-up, SFML is y-down)
sf::Vector2f to_px(float gx, float gy)
{
    return {gx + 300.f, 300.f - gy};
}

void drawScene(sf::RenderWindow& window)
{
    const sf::Color blue = sf::Color::Blue;

    // clicked points (10 px dots in GL -> r=5)
    for (int j = 0; j < i; j++)
    {
        sf::CircleShape dot(5.f);
        const sf::Vector2f p = to_px((float)tx[j], (float)ty[j]);
        dot.setPosition({p.x - 5.f, p.y - 5.f});
        dot.setFillColor(blue);
        window.draw(dot);
    }

    // segments in pairs (0,1) and (2,3): GL_LINES over tx[0..ff-1]
    const int ff = (i % 2 == 0) ? i : i - 1;
    sf::VertexArray lines(sf::PrimitiveType::Lines);
    for (int j = 0; j + 1 < ff; j += 2)
    {
        lines.append(sf::Vertex{to_px((float)tx[j], (float)ty[j]), blue});
        lines.append(sf::Vertex{to_px((float)tx[j + 1], (float)ty[j + 1]), blue});
    }
    window.draw(lines);

    // intersection point (red); the original drew it even for non-intersecting
    // segments, here only when isIntersect() actually found it
    if (i == 4 && intersected)
    {
        sf::CircleShape dot(5.f);
        const sf::Vector2f p = to_px((float)xx, (float)yy);
        dot.setPosition({p.x - 5.f, p.y - 5.f});
        dot.setFillColor(sf::Color::Red);
        window.draw(dot);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Interval intersection");
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
                if (i < 4)
                {
                    tx[i] = mousePressed->position.x - 300;
                    ty[i] = 300 - mousePressed->position.y;
                    i++;
                    if (i == 4)
                    {
                        intersected = isIntersect();
                        std::cout << "intersect=" << intersected << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        drawScene(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
