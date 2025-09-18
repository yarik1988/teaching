#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

// Constants adapted from the original code
const int MAX_DEPTH = 8;
const float POINT_SIZE = 0.1f; // The size of the "leaf" squares at the branch tips

/**
 * @brief Recursively generates the vertices for the fractal tree.
 *
 * Instead of drawing directly like the OpenGL version, this function adds vertices
 * for branches (as thick quads) and leaves (as small quads) to a single VertexArray.
 * This is a much more efficient rendering method.
 *
 * @param vertices The VertexArray to add the geometry to.
 * @param x The starting x-coordinate of the branch.
 * @param y The starting y-coordinate of the branch.
 * @param angle The angle of the branch.
 * @param len The length of the branch.
 * @param depth The current recursion depth.
 * @param ang_inc The angle increase for the next branches.
 * @param len_dec The length decay factor for the next branches.
 */
void fracTree(sf::VertexArray& vertices, float x, float y, float angle, float len, int depth, float ang_inc, float len_dec) {
    // Calculate the end point of the current branch
    const float x1 = x + len * std::cos(angle);
    const float y1 = y + len * std::sin(angle);

    // --- Recursive calls for the next level ---
    if (depth > 0) {
        // --- Draw the current branch ---
        // We simulate glLineWidth by creating a thin rectangle (a quad).
        // The trunk (high depth) is darker, and the twigs (low depth) are bright green.
        std::uint8_t green=255-depth*32;
        sf::Color branchColor(0,green, 0);

        // Thickness is proportional to the depth.
        const float thickness = depth / 120.0f;

        // Find the perpendicular vector to create the quad's corners
        sf::Vector2f direction(x1 - x, y1 - y);
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);
        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        // Add the 4 vertices of the quad to the main vertex array
        vertices.append({{x - offset.x, y - offset.y},branchColor});
        vertices.append({{x1 - offset.x, y1 - offset.y},branchColor});
        vertices.append({{x1 + offset.x, y1 + offset.y},branchColor});


        vertices.append({{x - offset.x, y - offset.y},branchColor});
        vertices.append({{x + offset.x, y + offset.y},branchColor});
        vertices.append({{x1 + offset.x, y1 + offset.y},branchColor});
        // Create the next two branches recursively
        fracTree(vertices, x1, y1, angle + ang_inc, len * len_dec, depth - 1, ang_inc, len_dec);
        fracTree(vertices, x1, y1, angle - ang_inc, len * len_dec, depth - 1, ang_inc, len_dec);
    } else {
        // --- Draw the "leaf" point at the end of a branch ---
        // This is also a small quad.
        const sf::Color leafColor(76, 76, 255); // Corresponds to glColor3f(0.3, 0.3, 1.0)
        const float ps = POINT_SIZE / 2.f;
        vertices.append(sf::Vertex({{x1 - ps, y1 - ps}, leafColor}));
        vertices.append(sf::Vertex({{x1 + ps, y1 - ps}, leafColor}));
        vertices.append(sf::Vertex({{x1 + ps, y1 + ps}, leafColor}));
        vertices.append(sf::Vertex({{x1 - ps, y1 - ps}, leafColor}));
        vertices.append(sf::Vertex({{x1 - ps, y1 + ps}, leafColor}));
        vertices.append(sf::Vertex({{x1 + ps, y1 + ps}, leafColor}));
    }
}

int main() {
    // --- 1. Window and View Setup ---
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800,800)), "Fractal Tree in SFML");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    // This view replicates gluOrtho2D(-5, 5, -5, 5).
    // Center is (0,0), size is 10x10. Height is negative to make positive-Y point up.
    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(10.f, -10.f));
    window.setView(view);

    // --- 2. Animation Variables ---
    float ka = 0.0f;
    const float ka_step = 0.01f;

    // A single VertexArray will hold all the geometry for the tree.
    // We use sf::Quads because each branch and leaf is a 4-sided polygon.
    sf::VertexArray treeVertices(sf::PrimitiveType::Triangles);


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        float ang_inc = (std::sin(ka + 2) + 2) / 5.0f;
        float len_dec = (std::sin(ka) + 10) / 12.0f;
        ka += ka_step;

        // --- 5. Rebuild Geometry for this Frame ---
        treeVertices.clear(); // Clear geometry from the last frame

        // Regenerate the four trees, rotated by 90 degrees.
        // The animated 'len_dec' is used as both the initial length and decay factor.
        fracTree(treeVertices, 0, 0, 0, len_dec, MAX_DEPTH, ang_inc, len_dec);
        fracTree(treeVertices, 0, 0, (float)M_PI / 2.0f, len_dec, MAX_DEPTH, ang_inc, len_dec);
        fracTree(treeVertices, 0, 0, (float)M_PI, len_dec, MAX_DEPTH, ang_inc, len_dec);
        fracTree(treeVertices, 0, 0, 3.0f * (float)M_PI / 2.0f, len_dec, MAX_DEPTH, ang_inc, len_dec);

        // --- 6. Draw Everything ---
        window.clear(sf::Color::White); // Original used a white background
        window.draw(treeVertices);     // Draw the entire vertex array in one go
        window.display();
    }

    return 0;
}