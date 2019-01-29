#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "pch.h"
#include "Character.h"
#include "Enemy.hpp"

class Audio
{
private:
	std::map<std::string, std::vector<sf::SoundBuffer>>soundCollection;
	std::map<std::string, std::vector<sf::Sound>> themSounds;
	sf::SoundBuffer buffer;
	sf::Music music;
	sf::Sound sound;
public:
	Audio(const std::string &filePath);

	void playSound(const std::string &key, const float volume);
	void soundMaker();
	void playSoundTwo(const std::string &key, const float volume);
	void playMusic(const std::string & song,const float volume);
	bool anySound();
};
#endif //AUDIO_HPP