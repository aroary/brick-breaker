#include "brick.h"

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dropDist(0, 5);
uniform_int_distribution<mt19937::result_type> xDist(0, 7);
uniform_int_distribution<mt19937::result_type> yDist(0, 3);

Brick::Brick(USI strength, USI x, USI y)
{
	this->strength = strength;
	this->x = x;
	this->y = y;
}

void Paddle::powerUps()
{
	// Every frame powerups get used up
	if (this->extention)
		this->extention--;
	if (this->lazer)
		this->lazer--;
	if (this->boost)
		this->boost--;
}

Drop::Drop(USI x, USI y)
{
	this->type = dropDist(rng);
	this->x = x;
	this->y = y;
}

Ball::Ball(USI angle, USI x, USI y)
{
	this->angle = angle;
	this->x = x;
	this->y = y;
}

Game::Game()
{
	this->initiateLevel();
	this->lives = 0;
}

void Game::initiateLevel()
{
	this->level++;
	
	this->bricks.clear();

	USI i = 0;
	while (i < 16)
	{
		USI x = xDist(rng);
		USI y = yDist(rng);
		USI strength = 1;
		
		if (i < 6)
			strength++;
		if (i < 8)
			strength++;
		
		USI j = 0;
		for (Brick brick: game.bricks)
			if (!(brick.x == x && brick.y == y))
				j++;
		
		if (j == i)
		{
			this->bricks.push_back(Brick(strength, x, y));
			i++;
		}
	}

	this->lives += 2;
}

Game game;