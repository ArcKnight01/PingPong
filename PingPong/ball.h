#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#define PI 3.14159265
using namespace sf;

class Ball {
private:

	Vector2f position;
	RectangleShape ballShape;
	float angle = ((rand() % 360) + 1) * PI  / 180; //(rand()%180+1)
	float mass = 1;
	float VelocityMagnitude = 5;
	float xVelocity = cos(angle) * VelocityMagnitude * 0.1f; //.5f
	float yVelocity = sin(angle) * VelocityMagnitude * 0.1f; //.5f
	const int windowW = 1024;
	const int windowH = 768;
	bool selected = false;
public:

	//overloaded constructor
	Ball(float startX, float startY);
	Ball();
	void setPosition(float posX, float posY);
	FloatRect getPosition();
	RectangleShape getShape();
	
	float getXVelocity();
	float getYVelocity();
	//bouncing off of sides:
	void reboundSides();
	void setAngle(float degrees);
	//bouncing off of paddle or top of screen:
	void reboundPaddleOrTop();
	void reboundOffBalls(Ball otherball);
	void hitBottom();
	void update();
	void setColor(sf::Color color);
	void selectBall();
	void deselectBall();

};