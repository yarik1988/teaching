// FloodFill.cpp : SFML 3 port of ../graphics_freeglut/FloodFill.cpp
// (left-click a cell: recursive flood fill turns its region red;
// original window title was the placeholder "Simple GLUT test")

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int DimX = 50;
const int DimY = 50;
int window_width = 600;
int window_height = 600;

// was struct rgb_color; sf::Color already has operator==/!=
sf::Color arr[DimX][DimY];
sf::Color old_color;
const sf::Color flood_color = sf::Color::Red;

void init()
{
    srand(time(NULL));
    for (int i = 0; i < DimX; i++)
        for (int j = 0; j < DimY; j++)
            arr[i][j] = (rand() % 2 == 0) ? sf::Color::White : sf::Color::Black;
}

void fill_rec(int x, int y)
{
    arr[x][y] = flood_color;
    if (x + 1 < DimX && arr[x + 1][y] == old_color) fill_rec(x + 1, y);
    if (x > 0 && arr[x - 1][y] == old_color) fill_rec(x - 1, y);
    if (y + 1 < DimY && arr[x][y + 1] == old_color) fill_rec(x, y + 1);
    if (y > 0 && arr[x][y - 1] == old_color) fill_rec(x, y - 1);
}

void flood_at(sf::Vector2i pos)
{
    const int mx = pos.x * DimX / window_width;
    const int my = (window_height - pos.y) * DimY / window_height; // GL ortho is y-up
    if (mx < 0 || mx >= DimX || my < 0 || my >= DimY) return;      // the GLUT original read out of bounds here
    old_color = arr[mx][my];
    if (old_color != flood_color) fill_rec(mx, my);
}

void drawCells(sf::RenderWindow& window)
{
    // glRecti(i,j,i+1,j+1) in gluOrtho2D(0,DimX,0,DimY): cell column i, row j
    // counted from the bottom -> flip rows for SFML's y-down pixels
    sf::VertexArray cells(sf::PrimitiveType::Triangles);
    const float cell_w = window_width / (float)DimX;
    const float cell_h = window_height / (float)DimY;
    for (int i = 0; i < DimX; i++)
        for (int j = 0; j < DimY; j++)
        {
            const float x0 = i * cell_w;
            const float y0 = (DimY - 1 - j) * cell_h;
            const float x1 = x0 + cell_w;
            const float y1 = y0 + cell_h;
            const sf::Color color = arr[i][j];
            cells.append(sf::Vertex{{x0, y0}, color});
            cells.append(sf::Vertex{{x1, y0}, color});
            cells.append(sf::Vertex{{x1, y1}, color});
            cells.append(sf::Vertex{{x0, y0}, color});
            cells.append(sf::Vertex{{x1, y1}, color});
            cells.append(sf::Vertex{{x0, y1}, color});
        }
    window.draw(cells);
}

int main()
{
    init();
    sf::RenderWindow window(sf::VideoMode({(unsigned)window_width, (unsigned)window_height}), "Flood fill");
    window.setPosition(sf::Vector2i(700, 100));
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                window_width = (int)resized->size.x;
                window_height = (int)resized->size.y;
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                    flood_at(mousePressed->position);
            }
        }

        window.clear(sf::Color::White);
        drawCells(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
