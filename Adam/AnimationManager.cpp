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
	while (in >> object >> action >> path) {
		Animation a(action);
		auto pos = animations.find(object);
		if (pos != animations.end()) {
			auto pos2 = pos->second.find(action);
			if (pos2 != pos->second.end()) {
				a = pos2->second;
			}
			else {
				pos->second[action] = a;
			}	
		}else{
			animations[object][action] = a;
		}
		animations[object][action].addFrame("assets/" + path);
	}
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
