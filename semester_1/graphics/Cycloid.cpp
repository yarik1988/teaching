// dyn_pict.cpp : SFML 3 port of ../graphics_freeglut/dyn_pict.cpp
// ("Brush draw tool" — a static parametric curve, no interaction)

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdint>

// the GLUT original draws in gluOrtho2D(-10,10,-10,10) on a 600x600 window:
// 30 px per graph unit, y axis flipped (GL is y-up, SFML is y-down)
sf::Vector2f to_px(float x, float y)
{
    return {x * 30.f + 300.f, 300.f - y * 30.f};
}

// the curve from display(): a "brush" path accumulating 0.05*(14sin(14i)+sin(1.3i))
sf::VertexArray buildBrushCurve()
{
    sf::VertexArray curve(sf::PrimitiveType::LineStrip);
    float x = -9.5f;
    float y = -0.5f;
    for (float i = 0; i < 100; i += 0.01f)
    {
        const float dx = 0.05f * (14 * sin(i * 14) + sin(i * 1.3f));
        const float dy = 0.05f * (14 * cos(i * 14) + cos(i * 1.3f));
        x += dx;
        y += dy;
        // glColor3f(0, fabs(dy), fabs(dx)) with 0..1 floats -> 0..255 bytes
        curve.append(sf::Vertex{to_px(x, y),
                                sf::Color(0, (std::uint8_t)(fabs(dy) * 255), (std::uint8_t)(fabs(dx) * 255))});
    }
    return curve;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Brush draw tool");
    window.setPosition(sf::Vector2i(0, 0));
    window.setVerticalSyncEnabled(true);

    const sf::VertexArray curve = buildBrushCurve();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(curve);
        window.display();
    }
    return EXIT_SUCCESS;
}
