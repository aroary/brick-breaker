#pragma once
#include <vector>

#define GAME_WIDTH 600
#define GAME_HEIGHT 400
#define BALLS 3
#define PADDLE_SPEED 10
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_SPEED 5
#define BALL_RADIUS 5

using std::vector;

typedef unsigned short int USI;

class Brick
{
public:
	Brick(USI);
	USI strength; // 0 indicates brick is destroyed
	USI drop; // The powerup that drops when brick is destroyed
	USI x;
	USI y;
};

class Paddle
{
public:
	USI speed = PADDLE_SPEED;
	USI width = PADDLE_WIDTH;
	USI height = PADDLE_HEIGHT;
	USI position = GAME_WIDTH / 2;
	
	// Powerups used each frame
	USI extention = 0;
	USI lazer = 0;
	USI boost = 0;
	void powerUps();
};

class Ball
{
public:
	Ball(USI);
	USI angle;
	USI speed = BALL_SPEED;
	USI radius = BALL_RADIUS;
};

class Game
{
public:
	Game();
	vector<Brick> bricks;
	vector<Ball> balls;
	Paddle paddle;
	USI lives = BALLS;
	USI score = 0;
	USI level = 0;
	void initiateLevel();
};

extern Game game;