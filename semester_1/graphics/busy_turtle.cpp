#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <optional>

using namespace std;

int main() {
    int n=10;

    int** a = new int*[n];
    for (int i = 0; i < n; ++i) {
        a[i] = new int[n];
    }

    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = rand() % 10 > 7 ? 1 : 0;
        }
    }

    int pos_x = rand() % n;
    int pos_y = rand() % n;
    int init_dir = rand() % 4; // 0-down, 1-left, 2-up, 3 right
    a[pos_x][pos_y] = 1 << (init_dir + 1);

    // SFML Window Setup
    const int cellSize = 40;
    sf::RenderWindow window(sf::VideoMode({(unsigned int)(n * cellSize), (unsigned int)(n * cellSize)}), "Busy Turtle");
    window.requestFocus();
    window.setFramerateLimit(2); // Adjust for animation speed

    bool isMoving = true;

    while (window.isOpen()) {
        // SFML 3.x event polling
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Turtle Traversal Logic
        if (isMoving) {
            int new_x = pos_x;
            int new_y = pos_y;

            switch (init_dir) {
                case 0: new_x = (pos_x + 1) % n; break;         // Down (increase row)
                case 1: new_y = (pos_y - 1 + n) % n; break;     // Left (decrease col)
                case 2: new_x = (pos_x - 1 + n) % n; break;     // Up (decrease row)
                case 3: new_y = (pos_y + 1) % n; break;         // Right (increase col)
                default: break;
            }

            if (a[new_x][new_y] == 1) {
                init_dir = (init_dir + 1) % 4;
                new_x = pos_x;
                new_y = pos_y;
            }

            int new_value = a[new_x][new_y] | (1 << (init_dir + 1));
            if (a[new_x][new_y] == new_value) {
                isMoving = false; // Stop animation when loop is detected
            } else {
                a[new_x][new_y] = new_value;
                pos_x = new_x;
                pos_y = new_y;
            }
        }

// Rendering
        window.clear(sf::Color::White);

        // Helper to draw a simple triangular arrow
        auto drawArrow = [&](int r, int c, int dir) {
            sf::ConvexShape arrow(3);
            arrow.setFillColor(sf::Color::Red);

            float cx = c * cellSize + cellSize / 2.0f;
            float cy = r * cellSize + cellSize / 2.0f;
            float sz = cellSize / 6.0f;
            float offset = sz; // How far to push the arrow away from the exact center

            // Define a base arrow pointing UP, shifted up by 'offset'
            arrow.setPoint(0, {-sz, -offset});
            arrow.setPoint(1, {sz, -offset});
            arrow.setPoint(2, {0.f, -offset - sz * 2.f}); // Arrow tip

            // Pin the rotation origin to the cell center
            arrow.setPosition({cx, cy});

            // Map direction to angle: 0->180 (Down), 1->270 (Left), 2->0 (Up), 3->90 (Right)
            float angle = ((dir + 2) % 4) * 90.0f;

            // SFML 3 uses sf::degrees for rotation
            arrow.setRotation(sf::degrees(angle));

            window.draw(arrow);
        };

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                sf::RectangleShape cell(sf::Vector2f({cellSize - 2.f, cellSize - 2.f}));
                cell.setPosition({(float)(j * cellSize), (float)(i * cellSize)});

                if (a[i][j] == 1) {
                    cell.setFillColor(sf::Color(80, 80, 80)); // Obstacle
                } else if (i == pos_x && j == pos_y) {
                    cell.setFillColor(sf::Color::Green); // Turtle head
                } else if (a[i][j] > 1) {
                    cell.setFillColor(sf::Color(150, 200, 150)); // Traversed path
                } else {
                    cell.setFillColor(sf::Color(220, 220, 220)); // Empty space
                }
                window.draw(cell);

                // Read the bits to draw arrows for every direction traversed here
                if (a[i][j] > 1) {
                    for (int k = 0; k < 4; ++k) {
                        if (a[i][j] & (1 << (k + 1))) {
                            drawArrow(i, j, k);
                        }
                    }
                }
            }
        }
        window.display();
    }

    // Memory cleanup
    for (int i = 0; i < n; ++i) {
        delete[] a[i];
    }
    delete[] a;

    return 0;
}