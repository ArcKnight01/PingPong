#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Paddle {
private:
	Vector2f position;
	RectangleShape paddleShape;

	float paddleSpeed = 0.5f;

public:
	//overloaded constructor:
	Paddle(float startX, float startY);
	FloatRect getPosition();
	RectangleShape getShape();

	void moveLeft();
	void moveRight();
	void update();

};