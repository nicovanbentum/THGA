#include "pch.h"
#include "Animation.h"

void Animation::addFrame(const std::string & location)
{
	textures.emplace_back(sf::Texture());
	textures.back().setSmooth(true);
	Collision::CreateTextureAndBitmask(textures.back(), location);
}

void Animation::addFrame(sf::Texture & texture)
{
	textures.emplace_back(texture);
}

std::shared_ptr<sf::Texture> Animation::nextFrame()
{
	const auto last_frame = textures.size() - 1;

	if (current_frame == last_frame && name == "DYINGright") {
		return textures.back();
	}

	current_frame = (current_frame + 1) % textures.size();

	return textures[current_frame];
}

std::shared_ptr<sf::Texture> Animation::getCurrentFrame()
{
	if (isDone() && name == "DYINGright") {
		return textures.back();
	}
	if (current_frame == textures.size()) {
		return textures.back();
	}
	else {
		return textures[(current_frame == -1 ) ? 0 : current_frame];
	}
}

std::shared_ptr<sf::Texture> Animation::getLastFrame()
{
	return textures.back();
}


void Animation::reset_animation()
{
	current_frame = 0;
}
void Animation::print() {
	std::cout << name << " ";
}

bool Animation::operator==(Animation &rhs) {
	return this->name == rhs.name;
}

bool Animation::operator!=(Animation &rhs) {
	return this->name != rhs.name;
}

bool Animation::operator!=(Animation rhs) {
	return this->name != rhs.name;
}