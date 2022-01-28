#include "pch.h"
#include "Drawable.h"

drawable::drawable(sf::Vector2f position, sf::Vector2f scale, std::shared_ptr<sf::Texture> texture):
	position(position),
	scale(scale),
	texture(texture),
	AABB(std::make_shared<sf::Texture>()),
	HB(std::make_shared<sf::Texture>())
{
	sprite.setPosition(position);
	sprite.setTexture(*texture);
	sprite.setScale(scale);
	Collision::CreateTextureAndBitmask(AABB, "assets/AABB.png");
	Collision::CreateTextureAndBitmask(HB, "assets/AABB_H.png");
}

sf::Vector2f drawable::getPosition() {
	return position;
}

void drawable::setPosition(sf::Vector2f newPos) {
	position = newPos;
	sprite.setPosition(position);
}

void drawable::setTexture(const std::string & textureFile) {
	Collision::CreateTextureAndBitmask(texture, textureFile);
	sprite.setTexture(*texture);
}

void drawable::setTexture(sf::Texture & t) {
	sprite.setTexture(t, true);
}

void drawable::setTexture(std::shared_ptr<sf::Texture> t) {
	sprite.setTexture(*t, true);
}

void drawable::draw(sf::RenderTarget &w) {
	w.draw(sprite);
}


