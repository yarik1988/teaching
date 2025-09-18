#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
using namespace std;
constexpr int window_height=600;
constexpr int window_width=800;
const float G=0.0001;
const float Dempf=0.8;
double mx,my;

class ball
{
sf::Vector2f v;
sf::Vector2f rad;
public:
sf::Sprite sprite;
explicit ball (sf::Texture& tex,float scale=0.1) : sprite(tex) {
    sprite.setScale(sf::Vector2f(scale, scale));
    rad.x = tex.getSize().x * scale;
    rad.y = tex.getSize().x * scale;
    auto pos_x = (rand() / (float) RAND_MAX) * (window_width - rad.x);
    auto pos_y = (rand() / (float) RAND_MAX) * (window_height - rad.y);
    sprite.setPosition(sf::Vector2f(pos_x, pos_y));
    v.x = (rand() / (float) RAND_MAX - 0.5) / 100.0;
    v.y = (rand() / (float) RAND_MAX - 0.5) / 100.0;
}

void re_eval(sf::Vector2f& gravity)
{
    auto new_pos=sprite.getPosition()+v;
    v+=gravity;
    if (new_pos.x<0) {v.x=fabs(v.x)*Dempf;new_pos.x=0;};
    if (new_pos.y<0) {v.y=fabs(v.y)*Dempf;new_pos.y=0;};
    if (new_pos.x>window_width-rad.x) {v.x=-fabs(v.x)*Dempf;new_pos.x=window_width-rad.x;}
    if (new_pos.y>window_height-rad.y) {v.y=-fabs(v.y)*Dempf;new_pos.y=window_height-rad.y;}
    sprite.setPosition(new_pos);
}
};

int main()
{
    sf::Vector2f gravity(0.0,0.0);
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(window_width,window_height)), "My window",sf::Style::Titlebar | sf::Style::Close);
    sf::Texture ball_tex;
    if (!ball_tex.loadFromFile("../res/ball.png")) return -1;
    sf::Texture bk_tex;
    if (!bk_tex.loadFromFile("../res/tiles.bmp")) return -1;
    bk_tex.setRepeated(true);
    sf::Sprite bk_sprite(bk_tex);
    bk_sprite.setTextureRect(sf::IntRect(sf::Vector2(0, 0),sf::Vector2(window_width,window_height)));
    std::vector<ball> ball_bunch;
    for (int i=0;i<100;i++) ball_bunch.emplace_back(ball_tex);
    sf::Vector2i mm(0,0);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode==sf::Keyboard::Scan::Right) gravity=sf::Vector2f(G,0);
                if (keyPressed->scancode==sf::Keyboard::Scan::Left) gravity=sf::Vector2f(-G,0);
                if (keyPressed->scancode==sf::Keyboard::Scan::Up) gravity=sf::Vector2f(0,-G);
                if (keyPressed->scancode==sf::Keyboard::Scan::Down) gravity=sf::Vector2f(0,G);
                if (keyPressed->scancode==sf::Keyboard::Scan::Space) gravity=sf::Vector2f(0,0);
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                mm=sf::Vector2i(mouseMoved->position.x%static_cast<int>(bk_tex.getSize().x),mouseMoved->position.y%static_cast<int>(bk_tex.getSize().y));
            }
        }
        window.clear(sf::Color::Black);
        bk_sprite.setTextureRect(sf::IntRect(sf::Vector2(mm.x, mm.y),sf::Vector2( mm.x+window_width,window_height+mm.y)));
        window.draw(bk_sprite);
        for (auto & it : ball_bunch)
        {
            window.draw(it.sprite);
            it.re_eval(gravity);
        }
        window.display();
    }
return 0;
}
