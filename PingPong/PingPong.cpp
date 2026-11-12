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
// State machine: which screen the game is on.
//   MENU     --START (click / Enter)-------------> PLAYING
//   PLAYING  --ball gets past a paddle-----------> GAMEOVER
//   GAMEOVER --RESTART (click / R)---------------> PLAYING  (fresh game)
//   GAMEOVER --MENU    (click / M)---------------> MENU     (fresh game waiting)
enum GameState {
	MENU,
	PLAYING,
	GAMEOVER
};

// A simple clickable button: a rectangle with its text centered on it.
struct Button {
	RectangleShape box;
	Text label;

	void setup(const Font& font, const string& text, float centerX, float centerY) {
		box.setSize(Vector2f(260, 90));
		box.setOrigin(box.getSize().x / 2, box.getSize().y / 2);
		box.setPosition(centerX, centerY);
		box.setOutlineColor(Color::White);
		box.setOutlineThickness(4);

		label.setFont(font);
		label.setString(text);
		label.setCharacterSize(48);
		label.setFillColor(Color::White);
		FloatRect bounds = label.getLocalBounds();
		label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
		label.setPosition(centerX, centerY);
	}
	bool contains(Vector2f point) {
		return box.getGlobalBounds().contains(point);
	}
	void draw(RenderWindow& window, Vector2f mouse) {
		box.setFillColor(contains(mouse) ? Color(60, 60, 255) : Color(0, 0, 160)); //lighter when hovered
		window.draw(box);
		window.draw(label);
	}
};

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
	const int numBalls = 1;
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
	GameState gameState = MENU; // start on the menu screen

	//MENU: one START button in the middle of the window
	Button startButton;
	startButton.setup(Candal, "START", windowWidth / 2, windowHeight / 2);

	//GAMEOVER: "GAME OVER", who missed, and RESTART / MENU buttons side by side
	Text gameOverText;
	gameOverText.setFont(Candal);
	gameOverText.setString("GAME OVER");
	gameOverText.setCharacterSize(80);
	gameOverText.setFillColor(Color::White);
	FloatRect gameOverBounds = gameOverText.getLocalBounds();
	gameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2, gameOverBounds.top + gameOverBounds.height / 2);
	gameOverText.setPosition(windowWidth / 2, windowHeight / 2 - 150);

	Text resultText; //set when the game ends (who missed + scores)
	resultText.setFont(Candal);
	resultText.setCharacterSize(30);
	resultText.setFillColor(Color::White);

	Button restartButton;
	restartButton.setup(Candal, "RESTART", windowWidth / 2 - 170, windowHeight / 2 + 60);
	Button menuButton;
	menuButton.setup(Candal, "MENU", windowWidth / 2 + 170, windowHeight / 2 + 60);

	//dark see-through layer drawn over the frozen game on the GAMEOVER screen
	RectangleShape dimOverlay(Vector2f(windowWidth, windowHeight));
	dimOverlay.setFillColor(Color(0, 0, 0, 150));

	//puts paddles, balls and scores back to the start of a game
	auto resetGame = [&]() {
		paddle = Paddle(windowWidth / 2, windowHeight - 20 - 20);
		enemyPaddle = Paddle(windowWidth / 2, 20);
		for (int i = 0; i < numBalls; i++) {
			balls[i].hitBottom(); //back to the center with a new random direction
		}
		score = 0;
		enemyScore = 0;
	};
	//ends the game and fills in the result line
	auto endGame = [&](const string& whoMissed) {
		resultText.setString(whoMissed + "   (bottom: " + to_string(score) + "   top: " + to_string(enemyScore) + ")");
		FloatRect bounds = resultText.getLocalBounds();
		resultText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
		resultText.setPosition(windowWidth / 2, windowHeight / 2 - 60);
		gameState = GAMEOVER;
	};

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
			bool leftClick = (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left);
			Vector2f click;
			if (leftClick) {
				click = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
			}
			bool keyPressed = (event.type == Event::KeyPressed);

			if (gameState == MENU) {
				//START (click it, or press Enter) -> PLAYING
				if ((leftClick && startButton.contains(click)) || (keyPressed && event.key.code == Keyboard::Enter)) {
					gameState = PLAYING;
				}
			}
			else if (gameState == GAMEOVER) {
				//RESTART (click it, or press R) -> PLAYING with a fresh game
				if ((leftClick && restartButton.contains(click)) || (keyPressed && event.key.code == Keyboard::R)) {
					resetGame();
					gameState = PLAYING;
				}
				//MENU (click it, or press M) -> back to the START screen
				else if ((leftClick && menuButton.contains(click)) || (keyPressed && event.key.code == Keyboard::M)) {
					resetGame();
					gameState = MENU;
				}
			}
		}

		Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

		//MENU screen: only the START button; the game waits until it is clicked
		if (gameState == MENU) {
			window.clear(Color(0, 0, 255)); //same blue background as the game
			startButton.draw(window, mouse);
			window.display();
			continue;
		}

		//GAMEOVER screen: the frozen game, dimmed, with the result and RESTART / MENU
		if (gameState == GAMEOVER) {
			window.clear(Color(0, 0, 255));
			window.draw(scoreText);
			window.draw(enemyScoreText);
			window.draw(paddle.getShape());
			window.draw(enemyPaddle.getShape());
			window.draw(dimOverlay);
			window.draw(gameOverText);
			window.draw(resultText);
			restartButton.draw(window, mouse);
			menuButton.draw(window, mouse);
			window.display();
			continue;
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

			//ball got past the top paddle -> GAMEOVER
			if (balls[i].getPosition().top < 0) {
				endGame("The top paddle missed!");
				break;
			}

			//ball got past the bottom paddle -> GAMEOVER
			if (balls[i].getPosition().top > windowHeight ) {
				endGame("The bottom paddle missed!");
				break;
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