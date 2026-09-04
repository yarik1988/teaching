#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#define DimX 500
#define DimY 500
int Width = 600;
int Height = 600;
bool arr[DimX][DimY];

void init()
{
    srand(time(NULL));
    for (int i = 0; i < DimX; i++)
        for (int j = 0; j < DimY; j++) {
            if (i == 0 || j == 0 || i == DimX - 1 || j == DimY - 1) arr[i][j] = false;
            else arr[i][j] = rand() % 2 == 0;
        }
}

void one_step()
{
    static bool new_arr[DimX][DimY];
    for (int i = 0; i < DimX; i++)
        for (int j = 0; j < DimY; j++)
        {
            if (i == 0 || j == 0 || i == DimX - 1 || j == DimY - 1) new_arr[i][j] = false;
            else
            {
                int num_neighbours = arr[i - 1][j] + arr[i + 1][j] + arr[i][j - 1] + arr[i][j + 1] + arr[i - 1][j - 1] + arr[i + 1][j + 1] + arr[i + 1][j - 1] + arr[i - 1][j + 1];
                if (arr[i][j])
                    new_arr[i][j] = (num_neighbours >= 2 && num_neighbours <= 3);
                else
                    new_arr[i][j] = num_neighbours == 3;
            }
        }

    for (int i = 1; i < DimX - 1; i++)
        for (int j = 1; j < DimY - 1; j++)
            arr[i][j] = new_arr[i][j];
}

void draw(sf::VertexArray& vertices)
{
    // SFML 3 has no Quads primitive: each cell is emitted as two triangles
    vertices.clear();
    const float cellW = float(Width) / DimX;
    const float cellH = float(Height) / DimY;
    for (int i = 0; i < DimX; i++)
        for (int j = 0; j < DimY; j++)
        {
            if (!arr[i][j]) continue;
            const float x0 = i * cellW;
            const float y0 = float(Height) - (j + 1) * cellH;
            const float x1 = x0 + cellW;
            const float y1 = y0 + cellH;
            vertices.append(sf::Vertex{sf::Vector2f(x0, y0), sf::Color::Black});
            vertices.append(sf::Vertex{sf::Vector2f(x1, y0), sf::Color::Black});
            vertices.append(sf::Vertex{sf::Vector2f(x1, y1), sf::Color::Black});
            vertices.append(sf::Vertex{sf::Vector2f(x0, y0), sf::Color::Black});
            vertices.append(sf::Vertex{sf::Vector2f(x1, y1), sf::Color::Black});
            vertices.append(sf::Vertex{sf::Vector2f(x0, y1), sf::Color::Black});
        }
}

int main()
{
    init();
    sf::RenderWindow window(sf::VideoMode({(unsigned int)(Width), (unsigned int)(Height)}), "Game of Life");
    window.setPosition(sf::Vector2i(700, 100));

    sf::VertexArray vertices(sf::PrimitiveType::Triangles);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        one_step();
        draw(vertices);

        window.clear(sf::Color::White);
        window.draw(vertices);
        window.display();
    }
    return EXIT_SUCCESS;
}
