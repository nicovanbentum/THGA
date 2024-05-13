#include "pch.h"
#include "Animation.h"

void Animation::addFrame(const std::string& location)
{
	textures.push_back(std::make_shared<sf::Texture>());
	textures.back()->setSmooth(true);
	const auto index = textures.size() - 1;

	Collision::CreateTextureAndBitmask(textures[index], location);
}

std::future<void> Animation::addFrameAsync(const std::string & location)
{
	textures.push_back(std::make_shared<sf::Texture>());
	textures.back()->setSmooth(true);
	const auto index = textures.size() - 1;

	return std::async(std::launch::async, [index, location, this]() {
		Collision::CreateTextureAndBitmask(textures[index], location);
	});
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