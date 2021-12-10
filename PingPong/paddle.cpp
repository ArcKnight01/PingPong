#include "paddle.h";

Paddle::Paddle(float startX, float startY) {
	position.x = startX;
	position.y = startY;

	paddleShape.setSize(sf::Vector2f(100, 10));
	paddleShape.setPosition(position);

}
FloatRect Paddle::getPosition() {
	return paddleShape.getGlobalBounds();
}
RectangleShape Paddle::getShape() {
	return paddleShape;
}

void Paddle::moveLeft() {
	position.x -= paddleSpeed;
}
void Paddle::moveRight() {
	position.x += paddleSpeed;
}

void Paddle::update() {
	paddleShape.setPosition(position);
}