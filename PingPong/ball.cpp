#include "ball.h"
Ball::Ball(float startX, float startY) {
	position.x = startX;
	position.y = startY;

	ballShape.setSize(sf::Vector2f(15, 15));
	ballShape.setPosition(position);	
	ballShape.setOutlineThickness(0.f);
	ballShape.setOutlineColor(sf::Color::Green);
	ballShape.setFillColor(sf::Color::Red);
	
 }
Ball::Ball() {
	position.x = windowH/2;
	position.y = windowW/2; //or 1

	ballShape.setSize(sf::Vector2f(15, 15));
	ballShape.setPosition(position);
	ballShape.setOutlineThickness(0.f);
	ballShape.setOutlineColor(sf::Color::Green);
	ballShape.setFillColor(sf::Color::White);
}
FloatRect Ball::getPosition() {
	return ballShape.getGlobalBounds();

}
RectangleShape Ball::getShape() {
	return ballShape;
}
float Ball::getXVelocity() {
	return xVelocity;
}
float Ball::getYVelocity() {
	return yVelocity;
}
void Ball::reboundSides() { //bouncing off sides.
	xVelocity = -xVelocity;
}
void Ball::setPosition(float posX, float posY) {
	position.x = posX;
	position.y = posY;
	ballShape.setSize(sf::Vector2f(15, 15));
	ballShape.setPosition(position);
}
void Ball::setAngle(float degrees) {
	angle = degrees;
	VelocityMagnitude = 5;
	xVelocity = cos(angle) * VelocityMagnitude * 0.1f; //.5f
	yVelocity = sin(angle) * VelocityMagnitude * 0.1f; //.5f
	selected = false;
}
void Ball::reboundPaddleOrTop() {
	int paddlethickness = 15;
	if (position.y < windowH/2){
		position.y += (paddlethickness + (yVelocity * 30));
	}
	else {
		position.y -= (paddlethickness + (yVelocity * 30));
	}
	yVelocity = -yVelocity;
}
void Ball::reboundOffBalls(Ball otherball) {
	position.x += 20 + ((rand() % 15) + 1);
	position.y += 20 + ((rand() % 15) + 1);

	float otherXVelocity = otherball.getXVelocity();
	float otherYVelocity = otherball.getYVelocity();
	xVelocity = -xVelocity;
	yVelocity = -yVelocity;
	
}
void Ball::setColor(sf::Color color) {
	ballShape.setFillColor(sf::Color(color));
}
void Ball::selectBall() {
	 selected = true;
}
void Ball::deselectBall() {
	selected = false;
}
void Ball::hitBottom() {
	position.y = (windowH/2);
	position.x = (windowW/2);
	float newAngle = ((rand() % 360) + 1) * PI / 180;
	if (newAngle > 60 && newAngle < 90) {
		newAngle = 60;
	}
	else if (newAngle >= 90 && newAngle < 120) {
		newAngle = 120;
	}
	else if (newAngle > 240 && newAngle < 270) {
		newAngle = 240;
	}
	else if (newAngle >= 270 && newAngle < 300) {
		newAngle = 300;
	}
	setAngle(newAngle);	
}
void Ball::update() {
	if (selected) {
		ballShape.setOutlineThickness(3.f);
		ballShape.setOutlineColor(sf::Color::Green);
	}
	else {
		ballShape.setOutlineThickness(0.f);
	}
	angle = atan(yVelocity / xVelocity);
	
	position.x += xVelocity;
	position.y += yVelocity;
	ballShape.setPosition(position);
}