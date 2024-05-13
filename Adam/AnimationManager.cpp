#include "pch.h"
#include "AnimationManager.h"
#include "Animation.h"


AnimationManager::AnimationManager(const std::string & s)
{
	std::ifstream in(s, std::ios::in | std::ios::binary);
	if (!in) {
		std::cout << "Couldn't open " << s << '\n';
	}
	std::string object, action, path = "";

	sf::Clock timer;

	std::string line;
	std::vector<std::string> lines;

	while (std::getline(in, line)) {
		lines.push_back(line);
	}

	std::mutex mutex;
	std::vector<std::future<void>> futures;

	for(const auto& line : lines) {
		auto ss = std::stringstream(line);
		ss >> object >> action >> path;

		animations.try_emplace(object);
		animations[object].try_emplace(action, action);

		std::scoped_lock lock(mutex);
		futures.push_back(animations[object][action].addFrameAsync("assets/" + path));
		// animations[object][action].addFrame("assets/" + path);

		std::cout << "assets/" << path << '\n';
	}
	
	for (const auto& future : futures) {
		future.wait();
	}

	std::cout << "AnimationManager took " << timer.getElapsedTime().asMilliseconds() << " ms." << '\n';
}

void AnimationManager::print()
{
	for (const auto& elem : animations)
	{
		for (const auto& m : elem.second) {
			std::cout << elem.first << " " << m.first << " " << m.second.textures.size() << '\n';
		}
		
	}
}
