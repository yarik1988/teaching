// Graph_SFML.cpp : SFML 3 port of graphics_freeglut/Graph.cpp
// (ImGui-SFML replaces the GLUT + ImGui-OpenGL2 backend)

#include <SFML/Graphics.hpp>
#include "imgui-SFML.h"
#include "imgui.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
#include <set>
#include <vector>

const int gl_width = 100;
const int gl_height = 100;
int window_width = 800;
int window_height = 800;
bool is_oriented = false;
int vertice_number = 10;
int link_prob = 50;

/********************/
struct Node
{
    float x, y;
    double dist;
    Node* precessor;
    std::vector<Node*> pts;
    Node() : precessor(nullptr) { dist = -1; }
};

Node* move_node = nullptr;
std::vector<Node*> graph;
std::set<Node*> pointed_vertices;

// clear path data
void path_clear()
{
    for (int i = 0; i < (int)graph.size(); i++)
    {
        graph[i]->precessor = nullptr;
        graph[i]->dist = -1;
    }
}

float norm(float x, float y)
{
    return sqrtf(x * x + y * y);
}

// arrow model: the GLUT port's triangle strip (v0..v3) in graph units,
// tip at origin, wings trailing towards -y
const float arrow_model[8] = {
    -1.5f, -7.5f,
    0.0f,  -5.0f,
    0.0f,   0.0f,
    1.5f,  -7.5f,
};

// appends the arrow head at graph-space (x,y), rotated by ang, as two triangles
// mapped to window pixels (sx/sy: pixels per graph unit, x and y axis)
void drawArrowHead(sf::VertexArray& arrows, float x, float y, float ang, float sx, float sy, float scale = 1)
{
    float vert[8];

    for (int i = 0; i < 8; i += 2)
    {
        vert[i] = arrow_model[i] * cosf(ang) + arrow_model[i + 1] * sinf(ang);
        vert[i + 1] = -arrow_model[i] * sinf(ang) + arrow_model[i + 1] * cosf(ang);
    }

    for (int i = 0; i < 8; i += 2)
    {
        vert[i] = vert[i] * scale + x;
        vert[i + 1] = vert[i + 1] * scale + y;
    }

    const sf::Color color(0, 128, 0); // glColor3f(0, 0.5f, 0)
    for (int idx : {0, 1, 2, 1, 2, 3})
        arrows.append(sf::Vertex{sf::Vector2f(vert[2 * idx] * sx, vert[2 * idx + 1] * sy), color});
}

void drawGraph(sf::RenderWindow& window)
{
    const sf::Vector2u wsz = window.getSize();
    const float sx = wsz.x / (float)gl_width;
    const float sy = wsz.y / (float)gl_height;

    const sf::Color node_color(25, 25, 25);  // glColor3f(0.1, 0.1, 0.1)
    const sf::Color edge_color(0, 0, 255);   // glColor3f(0, 0, 1)

    sf::VertexArray edges(sf::PrimitiveType::Lines);
    sf::VertexArray arrows(sf::PrimitiveType::Triangles);

    for (int i = 0; i < (int)graph.size(); i++)
        for (int j = 0; j < (int)graph[i]->pts.size(); j++)
        {
            const float begx = graph[i]->x;
            const float begy = graph[i]->y;
            const float endx = graph[i]->pts[j]->x;
            const float endy = graph[i]->pts[j]->y;
            edges.append(sf::Vertex{sf::Vector2f(begx * sx, begy * sy), edge_color});
            edges.append(sf::Vertex{sf::Vector2f(endx * sx, endy * sy), edge_color});
            const float ang = atan2(endx - begx, endy - begy);
            if (is_oriented)
                drawArrowHead(arrows, endx, endy, ang, sx, sy, 0.5f);
        }

    // vertices (5 px dots)
    for (int i = 0; i < (int)graph.size(); i++)
    {
        sf::CircleShape dot(2.5f);
        dot.setPosition({graph[i]->x * sx - 2.5f, graph[i]->y * sy - 2.5f});
        dot.setFillColor(node_color);
        window.draw(dot);
    }

    window.draw(edges);
    window.draw(arrows);

    // selected vertices (10 px red dots)
    for (std::set<Node*>::iterator it = pointed_vertices.begin(); it != pointed_vertices.end(); it++)
    {
        sf::CircleShape dot(5.f);
        dot.setPosition({(*it)->x * sx - 5.f, (*it)->y * sy - 5.f});
        dot.setFillColor(sf::Color::Red);
        window.draw(dot);
    }

    // shortest path
    if (pointed_vertices.size() == 2)
    {
        sf::VertexArray path(sf::PrimitiveType::Lines);
        Node* curr = *(++pointed_vertices.begin());
        while (curr->precessor != nullptr)
        {
            path.append(sf::Vertex{sf::Vector2f(curr->x * sx, curr->y * sy), sf::Color::Red});
            path.append(sf::Vertex{sf::Vector2f(curr->precessor->x * sx, curr->precessor->y * sy), sf::Color::Red});
            curr = curr->precessor;
        }
        window.draw(path);
    }
}

Node* find_node(int x, int y)
{
    for (int i = 0; i < (int)graph.size(); i++)
    {
        const float tx = x * gl_width / (float)window_width;
        const float ty = y * gl_height / (float)window_height;
        if (norm(graph[i]->x - tx, graph[i]->y - ty) < 2) return graph[i];
    }
    return nullptr;
}

void wide_search()
{
    if (pointed_vertices.size() != 2) return;
    path_clear();
    std::set<Node*> cur_nodes;

    cur_nodes.insert(*pointed_vertices.begin());
    (*pointed_vertices.begin())->dist = 0;

    while (cur_nodes.size() > 0)
    {
        Node* curr = *cur_nodes.begin();
        cur_nodes.erase(cur_nodes.begin());
        for (int i = 0; i < (int)curr->pts.size(); i++)
        {
            const float node_len = norm(curr->x - curr->pts[i]->x, curr->y - curr->pts[i]->y);
            if (curr->pts[i]->dist == -1 || curr->pts[i]->dist > curr->dist + node_len)
            {
                curr->pts[i]->precessor = curr;
                curr->pts[i]->dist = curr->dist + node_len;
                cur_nodes.insert(curr->pts[i]);
            }
        }
    }
}

/***********************************************************/
void mouse_action(sf::Mouse::Button button, bool pressed, sf::Vector2i pos)
{
    if (button == sf::Mouse::Button::Left && pressed)
    {
        move_node = find_node(pos.x, pos.y);
        if (move_node) { pointed_vertices.insert(move_node); path_clear(); }
    }
    if (button == sf::Mouse::Button::Left && !pressed) move_node = nullptr;
    if (button == sf::Mouse::Button::Right && pressed)
    {
        std::set<Node*>::iterator it = pointed_vertices.find(find_node(pos.x, pos.y));
        if (it != pointed_vertices.end()) { pointed_vertices.erase(it); path_clear(); }
    }
}

/********************************************/
void mouse_move(sf::Vector2i pos)
{
    if (move_node != nullptr)
    {
        move_node->x = pos.x * gl_width / (float)window_width;
        move_node->y = pos.y * gl_height / (float)window_height;
    }
}

/******************************************/
void graph_clear()
{
    for (int i = 0; i < (int)graph.size(); i++) delete graph[i];
    graph.clear();
}

void graph_create(bool ori)
{
    graph_clear();
    pointed_vertices.clear();
    srand(time(NULL));
    for (int i = 0; i < vertice_number; i++)
    {
        Node* curr = new Node;
        curr->x = rand() % gl_width;
        curr->y = rand() % gl_height;
        graph.push_back(curr);
    }
    is_oriented = ori;
    if (ori)
        for (int i = 0; i < (int)graph.size(); i++)
        {
            for (int j = 0; j < (int)graph.size(); j++)
            {
                if (rand() % 100 < link_prob && j != i)
                    graph[i]->pts.push_back(graph[j]);
            }
        }
    else
        for (int i = 0; i < (int)graph.size(); i++)
        {
            for (int j = i + 1; j < (int)graph.size(); j++)
            {
                if (rand() % 100 < link_prob)
                {
                    graph[i]->pts.push_back(graph[j]);
                    graph[j]->pts.push_back(graph[i]);
                }
            }
        }
}

/******************************************/
void vertex_delete()
{
    path_clear();
    for (auto it = pointed_vertices.begin(); it != pointed_vertices.end(); it++)
    {
        for (auto i = graph.begin(); i < graph.end(); i++)
        {
            auto itr = std::find((*i)->pts.begin(), (*i)->pts.end(), *it);
            if (itr != (*i)->pts.end())
                (*i)->pts.erase(itr);
        }
        delete *it; // the GLUT original leaked the nodes here
    }
    std::vector<Node*> new_graph;
    for (auto i = graph.begin(); i < graph.end(); i++)
        if (pointed_vertices.find(*i) == pointed_vertices.end())
            new_graph.push_back(*i);
    graph = new_graph;
    pointed_vertices.clear();
}

/********************************/
int main()
{
    sf::RenderWindow window(sf::VideoMode({(unsigned int)window_width, (unsigned int)window_height}), "Graph application");
    window.setPosition(sf::Vector2i(700, 100));
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "ImGui-SFML init failed\n";
        return EXIT_FAILURE;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui::StyleColorsDark();

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

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
                if (!io.WantCaptureMouse) // don't click "through" the toolbar
                    mouse_action(mousePressed->button, true, mousePressed->position);
            }
            else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                mouse_action(mouseReleased->button, false, mouseReleased->position);
            }
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                mouse_move(mouseMoved->position);
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("MAIN TOOLBAR", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SliderInt("Number of vertices", &vertice_number, 3, 30);
        ImGui::SliderInt("Connection probability", &link_prob, 0, 100);
        if (ImGui::Button("Create ordinary graph")) graph_create(false);
        if (ImGui::Button("Create oriented graph")) graph_create(true);
        if (ImGui::Button("Find shortest path")) wide_search();
        if (ImGui::Button("Delete selected vertices")) vertex_delete();
        ImGui::End();

        window.clear(sf::Color::White);
        drawGraph(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    graph_clear();
    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;
}
