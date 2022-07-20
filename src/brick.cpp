#include "brick.h"

Brick::Brick(USI strength)
{
	this->strength = strength;
	drop = 1;
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

Ball::Ball(USI angle)
{
	this->angle = angle;
}

Game::Game()
{
	this->initiateLevel();
}

void Game::initiateLevel()
{
	this->level++;
}

Game game;