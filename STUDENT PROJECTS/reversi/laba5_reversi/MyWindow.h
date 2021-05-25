#pragma once
#include <SFML/Graphics.hpp>
#include "Point.h"
using namespace sf;
using namespace std;

class MyWindow : public RenderWindow {
public:
    static const int SIZE = 64, line_width = 4;
    MyWindow(VideoMode mode, const String& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings()) : RenderWindow(mode, title, style, settings) {}
    
    void drawField() {
        RectangleShape line(Vector2f(SIZE * 8 + line_width, line_width));
        line.setFillColor(Color(105,105,105));
        for (int i = 0; i < 9; i++) {
            line.setPosition(0, i * SIZE);
            this->draw(line);
            line.rotate(90);
            line.setPosition(i * SIZE + line_width, 0);
            this->draw(line);
            line.rotate(-90);
        }
    }

    void drawPoint(long long x, long long y, int type) {
        if (!type) return;
        CircleShape shape(0.4 * SIZE);
        shape.setFillColor(type == 1 ? Color::Black : Color::White);
        shape.setPosition(((2 * y + 1) * SIZE + line_width - 0.8 * SIZE) / 2, ((2 * x + 1) * SIZE + line_width - 0.8 * SIZE) / 2);
        this->draw(shape);
    }

    void drawHighlightedPoint(long long x, long long y) {
        CircleShape shape(0.4 * SIZE);
        shape.setOutlineThickness(line_width);
        shape.setFillColor(Color::Green);
        shape.setOutlineColor(Color(255, 140, 0));
        shape.setPosition(((2 * y + 1) * SIZE + line_width - 0.8 * SIZE) / 2, ((2 * x + 1) * SIZE + line_width - 0.8 * SIZE) / 2);
        this->draw(shape);
    }

    void drawPoints(int** board) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                drawPoint(i, j, board[i][j]);
    }

    void drawHighlightedPoints(vector<Point> legalMoves) {
        for (Point p : legalMoves) {
            drawHighlightedPoint(p.x, p.y);
        }
    }

    void drawAll(int** board, vector<Point> legalMoves) {
        drawField();
        drawPoints(board);
        drawHighlightedPoints(legalMoves);
    }
};

