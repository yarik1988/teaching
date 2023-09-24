#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
using namespace std;
int window_height=600;
int window_width=800;
const float G=0.0001;
const float Dempf=0.8;
double mx,my;

class ball
{
sf::Vector2f v;
sf::Vector2f rad;
public:
sf::Sprite sprite;
ball (sf::Texture& tex,float scale=0.1)
{
    sprite.setTexture(tex);
    sprite.setScale(scale,scale);
    rad.x=tex.getSize().x*scale;
    rad.y=tex.getSize().x*scale;
    sprite.setPosition((rand()/(float)RAND_MAX)*(window_width-rad.x),(rand()/(float)RAND_MAX)*(window_height-rad.y));
    v.x=(rand()/(float)RAND_MAX-0.5)/100.0;
    v.y=(rand()/(float)RAND_MAX-0.5)/100.0;
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
    sf::RenderWindow window(sf::VideoMode(window_width,window_height), "My window",sf::Style::Titlebar | sf::Style::Close);
    sf::Texture ball_tex;
    if (!ball_tex.loadFromFile("../res/ball.png")) return -1;
    sf::Texture bk_tex;
    if (!bk_tex.loadFromFile("../res/tiles.bmp")) return -1;
    bk_tex.setRepeated(true);
    sf::Sprite bk_sprite;
    bk_sprite.setTexture(bk_tex);
    bk_sprite.setTextureRect({ 0, 0, window_width,window_height});
    std::vector<ball> ball_bunch;
    for (int i=0;i<100;i++) ball_bunch.emplace_back(ball_tex);
    sf::Vector2i mm(0,0);
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code==sf::Keyboard::Key::Right) gravity=sf::Vector2f(G,0);
                if (event.key.code==sf::Keyboard::Key::Left) gravity=sf::Vector2f(-G,0);
                if (event.key.code==sf::Keyboard::Key::Up) gravity=sf::Vector2f(0,-G);
                if (event.key.code==sf::Keyboard::Key::Down) gravity=sf::Vector2f(0,G);
                if (event.key.code==sf::Keyboard::Key::Space) gravity=sf::Vector2f(0,0);
            }
            if (event.type == sf::Event::MouseMoved)
            {
                mm=sf::Vector2i(event.mouseMove.x%(int)bk_tex.getSize().x,event.mouseMove.y%(int)bk_tex.getSize().y);
            }
        }
        window.clear(sf::Color::Black);
        bk_sprite.setTextureRect({ mm.x, mm.y, mm.x+window_width,window_height+mm.y});
        window.draw(bk_sprite);
        for (auto it=ball_bunch.begin();it!=ball_bunch.end();it++)
        {
            window.draw(it->sprite);
            it->re_eval(gravity);
        }
        window.display();
    }
return 0;
}
