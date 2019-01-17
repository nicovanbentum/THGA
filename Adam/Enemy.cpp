#include "Enemy.hpp"

Enemy::Enemy(sf::Vector2f position, sf::Vector2f scale, const std::string & textureFile, sf::Vector2f velocity, statistic health_c) :
	movable(position, scale, textureFile, velocity)
{
	health = health_c;
	Collision::CreateTextureAndBitmask(texture, textureFile);
	std::cout << position.x << " <charPosX";

	if (!font.loadFromFile("fonts/stranger.ttf"))
	{
		//std::cout << "error loading font" << std::endl;
	}
	update_info();

	for (int i = 0; i < 2; i++)
	{
		text[i].setFont(font);
		text[i].setOutlineColor(sf::Color::Black);
		text[i].setOutlineThickness(2.0f);
		text[i].setCharacterSize(24);
		text[i].setString(t[i]);
	}
	text[0].setFillColor(sf::Color::Red);
	text[1].setFillColor(sf::Color::White);
}


Enemy::~Enemy()
{
}

void Enemy::update_info()
{
	t[0] = std::to_string(health.current) + "/" + std::to_string(health.max) + '\n';
	t[1] = "  lvl: " + std::to_string(lvl);
	for (unsigned int k = 0; k < 2; k++)
	{
		text[k].setString(t[k]);

	}
}

void Enemy::update_info_pos(sf::RenderWindow & window)
{
	for (unsigned int j = 0; j < 2; j++)
	{
		text[j].setPosition(sf::Vector2f(position.x, position.y - 50 + j * 24));
		window.draw(text[j]);
		//std::cout << ;
	}
}