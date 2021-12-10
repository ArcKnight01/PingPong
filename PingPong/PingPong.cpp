//using SFML Template
#include "paddle.h"
#include "ball.h"
#include <iostream>
#include <string> 
#include <sstream>//for string classes
#include <cstdlib>// C standard lib
#include <cmath>
// Include Needed for SFML
#include <SFML/Graphics.hpp>
// Include Needed for Animations
#include <SFML/Graphics/Rect.hpp>
// Include Needed for Text
#include <SFML/Graphics/Text.hpp>
using namespace std;
using namespace sf;
/* DEFINE GAMESTATES HERE */

int main()
{ 
	//setting window
	const int windowWidth = 1024;
	const int windowHeight = 768;
	
	bool enemyIsAI = true;
	bool playerIsAI = false;

	// Create a Window to Display Graphics, Define Size and Window Name
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Ping Pong Game");
	/* CREATE OBJECTS AND ASSIGN OBJECT ATTRIBUTES HERE */
	Paddle paddle(windowWidth / 2, windowHeight - 20 -20);
	Paddle enemyPaddle(windowWidth / 2, 20);

	//Ball ball(windowWidth / 2, 1);
	const int numBalls = 30;
	Ball balls[numBalls];
	for (int i = 0; i < numBalls; i++) {
		balls[i].setPosition(windowWidth / 2 , windowHeight / 2);
	}
	int score = 0;
	int enemyScore = 0;

	string scoreString = "score: " + to_string(score);
	string enemyScoreString = "score: " + to_string(enemyScore);
	Text scoreText;
	Text enemyScoreText;
	Font Candal;

	Candal.loadFromFile("Font/Candal.ttf");
	
	scoreText.setFont(Candal);
	scoreText.setString(" ");
	scoreText.setPosition(windowWidth/ 2 + windowWidth/4, 50);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::White);

	enemyScoreText.setFont(Candal);
	enemyScoreText.setString(" ");
	enemyScoreText.setPosition(windowWidth / 2 + windowWidth / 4, 100);
	enemyScoreText.setCharacterSize(30);
	enemyScoreText.setFillColor(Color::Red);

	float ballOriginX[numBalls];
	float ballOriginY[numBalls];
	float distXfromEnemy[numBalls];
	float distYfromEnemy[numBalls];
	float distXfromPlayer[numBalls];
	float distYfromPlayer[numBalls];

	/* DEFINE VARIABLES HERE */
	bool enemyMovesLeft = false;
	bool enemyMovesRight = false;
	int nearestBallIndex = 0;
	int farthestBallIndex = 0;
	float closestdistance = 0;
	// Game loop: 
	//Run the Program while the Window is Open
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			// When X Button Clicked, Close Window
			if (event.type == Event::Closed) {
				window.close();
			}
			/* DEFINE EVENTS (i.e. KEY PRESS & RELEASE) HERE */
		}

		float enemyBackY = enemyPaddle.getPosition().top; // back of paddle
		float enemyFrontY = enemyBackY + 10; // front of paddle
		float enemyLeftX = enemyPaddle.getPosition().left; //left of paddle
		float enemyRightX = enemyLeftX + enemyPaddle.getPosition().width; //right of paddle
		float enemyCenterX = (enemyLeftX + enemyRightX) / 2;
		float playerFrontY = paddle.getPosition().top; // back of paddle
		float playerBackY = playerFrontY - 10; // front of paddle
		float playerLeftX = paddle.getPosition().left; //left of paddle
		float playerRightX = playerLeftX + paddle.getPosition().width; //right of paddle
		float playerCenterX = (playerLeftX + playerRightX) / 2;

		for (int i = 0; i < numBalls; i++) {
			ballOriginX[i] = balls[i].getPosition().left; //+ (balls[i].getPosition().width / 2);
			ballOriginY[i] = balls[i].getPosition().top;// +(balls[i].getPosition().height / 2);
			distXfromEnemy[i] = ballOriginX[i] - enemyCenterX;
			distYfromEnemy[i] = (ballOriginY[i] - enemyFrontY);
			distXfromPlayer[i] = ballOriginX[i] - playerCenterX;
			distYfromPlayer[i] = abs(ballOriginY[i] - playerFrontY);
		}
		for (int i = 0; i < numBalls; i++) {
			if (numBalls > 1) {
				if (distYfromEnemy[i] < distYfromEnemy[nearestBallIndex]) {
					nearestBallIndex = i;
				}
				else if (distYfromEnemy[i] == distYfromEnemy[nearestBallIndex]) {
					if (distXfromEnemy[i] <= distXfromEnemy[nearestBallIndex] ) {
						nearestBallIndex = i;
					}
				}

				if (distYfromPlayer[i] < distYfromPlayer[farthestBallIndex]) {
					farthestBallIndex = i;
				}
				else if (distYfromPlayer[i] == distYfromPlayer[farthestBallIndex]) {
					if (distXfromPlayer[i] <= distXfromPlayer[farthestBallIndex]) {
						farthestBallIndex = i;
					}
				}
			}
			else {
				nearestBallIndex = 0;
				farthestBallIndex = 0;
			}
			closestdistance = distYfromEnemy[nearestBallIndex];
		}
		for (int i = 0; i < numBalls; i++) {
			if (i == nearestBallIndex) {
				//balls[nearestBallIndex].selectBall();
				//balls[nearestBallIndex].setColor(sf::Color::Red);
			}
			else if (i == farthestBallIndex){
				//balls[farthestBallIndex].selectBall();
				//balls[farthestBallIndex].setColor(sf::Color::Black);
			}
			else {
				balls[i].deselectBall();
				balls[i].setColor(sf::Color::White);
			}
		}
		//balls[0].setColor(sf::Color::Yellow);
		//paddle movement 
		if (Keyboard::isKeyPressed(Keyboard::Num1)) {
			enemyIsAI = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2)) {
			playerIsAI = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num0)) {
			enemyIsAI = false;
			playerIsAI = false;
		}

		if (!playerIsAI){
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				paddle.moveLeft();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right)) {
				paddle.moveRight();
			}
		}
		else if (playerIsAI) {
			//now that nearest ball is found : figure out wheter ball dist is (-) or left of the paddle, or (+) or right of the paddle
			if (playerCenterX > ballOriginX[farthestBallIndex] && playerLeftX >= 0) {
				//ball is to left of paddle
				paddle.moveLeft();
			}
			else if (playerCenterX < ballOriginX[farthestBallIndex] && playerRightX <= windowWidth) {
				//ball is to right of paddle
				paddle.moveRight();
			}
		}
		 
		if (!enemyIsAI) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				enemyPaddle.moveLeft();
			}
			else if (Keyboard::isKeyPressed(Keyboard::D)) {
				enemyPaddle.moveRight();
			}
		}
		else if (enemyIsAI) {
			//now that nearest ball is found : figure out wheter ball dist is (-) or left of the paddle, or (+) or right of the paddle
			if (enemyCenterX > ballOriginX[nearestBallIndex] && enemyLeftX >= 0) {
				//ball is to left of paddle
				enemyPaddle.moveLeft();
			}
			else if (enemyCenterX < ballOriginX[nearestBallIndex] && enemyRightX <= windowWidth) {
				//ball is to right of paddle
				enemyPaddle.moveRight();
			}
		}
		


		/* DEFINE ACTIONS (i.e. MOVEMENT) HERE */
		scoreText.setPosition(windowWidth / 2 + windowWidth/4, 25);
		scoreString = "score: " + to_string(score);
		scoreText.setString(scoreString);

		enemyScoreText.setPosition(windowWidth / 2 - windowWidth/3, 25);
		enemyScoreString = "enemy score: " + to_string(enemyScore);
		enemyScoreText.setString(enemyScoreString);


		//ball hitting top of window
		for (int i = 0; i < numBalls; i++) {

				 enemyBackY = enemyPaddle.getPosition().top; // back of paddle
				 enemyFrontY = enemyBackY + 10; // front of paddle
				 enemyLeftX = enemyPaddle.getPosition().left; //left of paddle
				 enemyRightX = enemyLeftX + enemyPaddle.getPosition().width; //right of paddle
				 enemyCenterX = (enemyLeftX + enemyRightX) / 2;
				 playerFrontY = paddle.getPosition().top; // back of paddle
				 playerBackY = playerFrontY - 10; // front of paddle
				 playerLeftX = paddle.getPosition().left; //left of paddle
				 playerRightX = playerLeftX + paddle.getPosition().width; //right of paddle
				 playerCenterX = (playerLeftX + playerRightX) / 2;

			if (balls[i].getPosition().top < 0) {
				//|| balls[i].getPosition().top < enemyBackY
				balls[i].hitBottom();
				enemyScore = 0;
				//balls[i].reboundPaddleOrTop();
			}

			//ball hitting bottom
			if (balls[i].getPosition().top > windowHeight ) {//balls[i].getPosition().top > playerBackY
				
				balls[i].hitBottom();
				score = 0;

			}

			//ball hitting sides
			if (balls[i].getPosition().left < 0 || (balls[i].getPosition().left + balls[i].getPosition().width) > windowWidth) {
				balls[i].reboundSides();
			}

			//ball hitting paddle
			if (balls[i].getPosition().intersects(paddle.getPosition())) {
				if ((balls[i].getPosition().left < playerRightX || (balls[i].getPosition().left + balls[i].getPosition().width) < playerLeftX) && balls[i].getPosition().top <= playerBackY) {
					balls[i].reboundPaddleOrTop();
					score++;
					//score = 1000;
				}
				else if ((balls[i].getPosition().left >= playerRightX || (balls[i].getPosition().left + balls[i].getPosition().width) <= playerLeftX) && balls[i].getPosition().top  > playerBackY) {
					balls[i].reboundSides();
					score = 1000;
				}
				else {
					score = -500;
					balls[i].reboundPaddleOrTop();
				}
			}
			if (balls[i].getPosition().intersects(enemyPaddle.getPosition())) {
				if ((balls[i].getPosition().left < enemyRightX || (balls[i].getPosition().left + balls[i].getPosition().width) < enemyLeftX) && (balls[i].getPosition().top + balls[i].getPosition().height) >= enemyBackY) {
					balls[i].reboundPaddleOrTop();
					enemyScore++;
				}
				else if ((balls[i].getPosition().left >= enemyRightX || (balls[i].getPosition().left + balls[i].getPosition().width) <= enemyLeftX) && (balls[i].getPosition().top + balls[i].getPosition().height) < enemyBackY) {
					balls[i].reboundSides();
					enemyScore == 1000;
				}
				else {
					enemyScore = -500;
					balls[i].reboundPaddleOrTop();
				}
				
			}
			for (int otherBall = 0; otherBall < numBalls; otherBall++) {
				if (otherBall != i) {
					int thisBall = i;
					if (balls[thisBall].getPosition().intersects(balls[otherBall].getPosition())) {
						balls[i].reboundOffBalls(balls[otherBall]);
					}
				}
			}
		}
		// intersects() function of position object is used to interact with another object
		enemyPaddle.update();
		paddle.update();

		for (int i = 0; i < numBalls; i++) {
			balls[i].update();
		}
		
		

		window.clear(Color(0,0, 255)); //clear scrn and set background color to blue
		
		window.draw(scoreText);
		window.draw(enemyScoreText);

		window.draw(paddle.getShape());
		window.draw(enemyPaddle.getShape());

		for (int i = 0; i < numBalls; i++) {
			
			window.draw(balls[i].getShape());

			
		}
		/* DRAW OBJECTS HERE */
		window.display();
	}
}